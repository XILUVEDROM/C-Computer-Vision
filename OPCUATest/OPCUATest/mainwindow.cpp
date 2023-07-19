
#include "mainwindow.h"
#include "opcuamodel.h"
#include "certificatedialog.h"

#include <QCoreApplication>
#include <QDir>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextCharFormat>
#include <QPlainTextEdit>
#include <QTextBlock>
#include <QTreeView>
#include <QHeaderView>
#include < QtOpcUa >
#include <QOpcUaProvider>
#include <QOpcUaAuthenticationInformation>
#include <QOpcUaErrorState>

static MainWindow *mainWindowGlobal = nullptr;
static QtMessageHandler oldMessageHandler = nullptr;

static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (!mainWindowGlobal)
        return;

   QString message = "%1: %2";
   QString contextStr = " (%1:%2, %3)";
   QString typeString;

   if (type == QtDebugMsg)
       typeString = QObject::tr("Debug");
   else if (type == QtInfoMsg)
        typeString = QObject::tr("Info");
   else if (type == QtWarningMsg)
        typeString = QObject::tr("Warning");
   else if (type == QtCriticalMsg)
        typeString = QObject::tr("Critical");
   else if (type == QtFatalMsg)
        typeString = QObject::tr("Fatal");

   message = message.arg(typeString).arg(msg);
   contextStr = contextStr.arg(context.file).arg(context.line).arg(context.function);

   QColor color = Qt::black;
   if (type == QtFatalMsg || type == QtCriticalMsg)
       color = Qt::darkRed;
   else if (type == QtWarningMsg)
       color = Qt::darkYellow;

   // Logging messages from backends are sent from different threads and need to be
   // synchronized with the GUI thread.
   QMetaObject::invokeMethod(mainWindowGlobal, "log", Qt::QueuedConnection,
                             Q_ARG(QString, message),
                             Q_ARG(QString, contextStr),
                             Q_ARG(QColor, color));

   if (oldMessageHandler)
       oldMessageHandler(type, context, msg);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
  , mOpcUaPlugin(new QComboBox(this))
  , mHost(new QLineEdit(this))
  , mServers(new QComboBox(this))
  , mEndpoints(new QComboBox(this))
  , mFindServersButton(new QPushButton(tr("Find Servers"), this))
  , mGetEndpointsButton(new QPushButton(tr("Get Endpoints"), this))
  , mConnectButton(new QPushButton(tr("Connect"), this))
  , mLog(new QPlainTextEdit(this))
  , mTreeView(new QTreeView(this))
  , mOpcUaModel(new OpcUaModel(this))
  , mOpcUaProvider(new QOpcUaProvider(this))
  , mOpcUaClient(nullptr)
  , mClientConnected(false)
{
    int row = 0;
    mainWindowGlobal = this;

    auto grid = new QGridLayout;
    grid->addWidget(new QLabel(tr("Select OPC UA Backend:")), row, 0);
    grid->addWidget(mOpcUaPlugin, row, 1);
    grid->addWidget(new QLabel(tr("Select host to discover:")), ++row, 0);
    grid->addWidget(mHost, row, 1);
    grid->addWidget(mFindServersButton, row, 2);
    grid->addWidget(new QLabel(tr("Select OPC UA Server:")), ++row, 0);
    grid->addWidget(mServers, row, 1);
    grid->addWidget(mGetEndpointsButton, row, 2);
    grid->addWidget(new QLabel(tr("Select OPC UA Endpoint:")), ++row, 0);
    grid->addWidget(mEndpoints, row, 1);
    grid->addWidget(mConnectButton, row, 2);

    auto vbox = new QVBoxLayout;
    vbox->addLayout(grid);
    vbox->addWidget(mTreeView);
    vbox->addWidget(new QLabel(tr("Log:")));
    vbox->addWidget(mLog);

    auto widget = new QWidget;
    widget->setLayout(vbox);
    setCentralWidget(widget);

    mHost->setText("opc.tcp://localhost:48010");
    mEndpoints->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    updateUiState();

    mOpcUaPlugin->addItems(mOpcUaProvider->availableBackends());
    mLog->setReadOnly(true);
    mLog->setLineWrapMode(QPlainTextEdit::NoWrap);
    setMinimumWidth(800);
    mTreeView->setModel(mOpcUaModel);
    mTreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    mTreeView->setTextElideMode(Qt::ElideRight);
    mTreeView->setAlternatingRowColors(true);
    mTreeView->setSelectionBehavior(QAbstractItemView::SelectItems);

    if (mOpcUaPlugin->count() == 0) {
        mOpcUaPlugin->setDisabled(true);
        mConnectButton->setDisabled(true);
        QMessageBox::critical(this, tr("No OPCUA plugins available"), tr("The list of available OPCUA plugins is empty. No connection possible."));
    }

    connect(mFindServersButton, &QPushButton::clicked, this, &MainWindow::findServers);
    connect(mGetEndpointsButton, &QPushButton::clicked, this, &MainWindow::getEndpoints);
    connect(mConnectButton, &QPushButton::clicked, this, &MainWindow::connectToServer);
    oldMessageHandler = qInstallMessageHandler(&messageHandler);

    setupPkiConfiguration();

    m_identity = m_pkiConfig.applicationIdentity();
}

void MainWindow::setupPkiConfiguration()
{
    QString pkidir = QCoreApplication::applicationDirPath();
#ifdef Q_OS_WIN
    pkidir += "../";
#endif
    pkidir += "/pki";
    m_pkiConfig.setClientCertificateFile(pkidir + "/own/certs/opcuaviewer.der");
    m_pkiConfig.setPrivateKeyFile(pkidir + "/own/private/opcuaviewer.pem");
    m_pkiConfig.setTrustListDirectory(pkidir + "/trusted/certs");
    m_pkiConfig.setRevocationListDirectory(pkidir + "/trusted/crl");
    m_pkiConfig.setIssuerListDirectory(pkidir + "/issuers/certs");
    m_pkiConfig.setIssuerRevocationListDirectory(pkidir + "/issuers/crl");

    // create the folders if they don't exist yet
    createPkiFolders();
}

void MainWindow::createClient()
{
    if (mOpcUaClient == nullptr) {
        mOpcUaClient = mOpcUaProvider->createClient(mOpcUaPlugin->currentText());
        if (!mOpcUaClient) {
            const QString message(tr("Connecting to the given sever failed. See the log for details."));
            log(message, QString(), Qt::red);
            QMessageBox::critical(this, tr("Failed to connect to server"), message);
            return;
        }

        connect(mOpcUaClient, &QOpcUaClient::connectError, this, &MainWindow::showErrorDialog);
        mOpcUaClient->setApplicationIdentity(m_identity);
        mOpcUaClient->setPkiConfiguration(m_pkiConfig);

        if (mOpcUaClient->supportedUserTokenTypes().contains(QOpcUaUserTokenPolicy::TokenType::Certificate)) {
            QOpcUaAuthenticationInformation authInfo;
            authInfo.setCertificateAuthentication();
            mOpcUaClient->setAuthenticationInformation(authInfo);
        }

        connect(mOpcUaClient, &QOpcUaClient::connected, this, &MainWindow::clientConnected);
        connect(mOpcUaClient, &QOpcUaClient::disconnected, this, &MainWindow::clientDisconnected);
        connect(mOpcUaClient, &QOpcUaClient::errorChanged, this, &MainWindow::clientError);
        connect(mOpcUaClient, &QOpcUaClient::stateChanged, this, &MainWindow::clientState);
        connect(mOpcUaClient, &QOpcUaClient::endpointsRequestFinished, this, &MainWindow::getEndpointsComplete);
        connect(mOpcUaClient, &QOpcUaClient::findServersFinished, this, &MainWindow::findServersComplete);
    }
}

void MainWindow::findServers()
{
    QStringList localeIds;
    QStringList serverUris;
    QUrl url(mHost->text());

    updateUiState();

    createClient();
    // set default port if missing
    if (url.port() == -1) url.setPort(4840);

    if (mOpcUaClient) {
        mOpcUaClient->findServers(url, localeIds, serverUris);
        qDebug() << "Discovering servers on " << url.toString();
    }
}

void MainWindow::findServersComplete(const QVector<QOpcUaApplicationDescription> &servers, QOpcUa::UaStatusCode statusCode)
{
    QOpcUaApplicationDescription server;

    if (isSuccessStatus(statusCode)) {
        mServers->clear();
        for (const auto &server : servers) {
            QVector<QString> urls = server.discoveryUrls();
            for (const auto &url : qAsConst(urls))
                mServers->addItem(url);
        }
    }

    updateUiState();
}

void MainWindow::getEndpoints()
{
    mEndpoints->clear();
    updateUiState();

    if (mServers->currentIndex() >= 0) {
        const QString serverUrl = mServers->currentText();
        createClient();
        mOpcUaClient->requestEndpoints(serverUrl);
    }
}

void MainWindow::getEndpointsComplete(const QVector<QOpcUaEndpointDescription> &endpoints, QOpcUa::UaStatusCode statusCode)
{
    int index = 0;
    const char *modes[] = {
        "Invalid",
        "None",
        "Sign",
        "SignAndEncrypt"
    };

    if (isSuccessStatus(statusCode)) {
        mEndpointList = endpoints;
        for (const auto &endpoint : endpoints) {
            if (endpoint.securityMode() > sizeof(modes)) {
                qWarning() << "Invalid security mode";
                continue;
            }

            const QString EndpointName = QString("%1 (%2)")
                    .arg(endpoint.securityPolicy())
                    .arg(modes[endpoint.securityMode()]);
            mEndpoints->addItem(EndpointName, index++);
        }
    }

    updateUiState();
}

void MainWindow::connectToServer()
{
    if (mClientConnected) {
        mOpcUaClient->disconnectFromEndpoint();
        return;
    }

    if (mEndpoints->currentIndex() >= 0) {
        m_endpoint = mEndpointList[mEndpoints->currentIndex()];
        createClient();
        mOpcUaClient->connectToEndpoint(m_endpoint);
    }
}

void MainWindow::clientConnected()
{
    mClientConnected = true;
    updateUiState();

    connect(mOpcUaClient, &QOpcUaClient::namespaceArrayUpdated, this, &MainWindow::namespacesArrayUpdated);
    mOpcUaClient->updateNamespaceArray();
}

void MainWindow::clientDisconnected()
{
    mClientConnected = false;
    mOpcUaClient->deleteLater();
    mOpcUaClient = nullptr;
    mOpcUaModel->setOpcUaClient(nullptr);
    updateUiState();
}

void MainWindow::namespacesArrayUpdated(const QStringList &namespaceArray)
{
    if (namespaceArray.isEmpty()) {
        qWarning() << "Failed to retrieve the namespaces array";
        return;
    }

    disconnect(mOpcUaClient, &QOpcUaClient::namespaceArrayUpdated, this, &MainWindow::namespacesArrayUpdated);
    mOpcUaModel->setOpcUaClient(mOpcUaClient);
    mTreeView->header()->setSectionResizeMode(1 /* Value column*/, QHeaderView::Interactive);
}

void MainWindow::clientError(QOpcUaClient::ClientError error)
{
    qDebug() << "Client error changed" << error;
}

void MainWindow::clientState(QOpcUaClient::ClientState state)
{
    qDebug() << "Client state changed" << state;
}

void MainWindow::updateUiState()
{
    // allow changing the backend only if it was not already created
    mOpcUaPlugin->setEnabled(mOpcUaClient == nullptr);
    mConnectButton->setText(mClientConnected?tr("Disconnect"):tr("Connect"));

    if (mClientConnected) {
        mHost->setEnabled(false);
        mServers->setEnabled(false);
        mEndpoints->setEnabled(false);
        mFindServersButton->setEnabled(false);
        mGetEndpointsButton->setEnabled(false);
        mConnectButton->setEnabled(true);
        mConnectButton->setText(tr("Disconnect"));
    } else {
        mHost->setEnabled(true);
        mServers->setEnabled(mServers->count() > 0);
        mEndpoints->setEnabled(mEndpoints->count() > 0);

        mFindServersButton->setDisabled(mHost->text().isEmpty());
        mGetEndpointsButton->setEnabled(mServers->currentIndex() != -1);
        mConnectButton->setEnabled(mEndpoints->currentIndex() != -1);
        mConnectButton->setText(tr("Connect"));
    }

    if (!mOpcUaClient) {
        mServers->setEnabled(false);
        mEndpoints->setEnabled(false);
        mGetEndpointsButton->setEnabled(false);
        mConnectButton->setEnabled(false);
    }
}

void MainWindow::log(const QString &text, const QString &context, QColor color)
{
    auto cf = mLog->currentCharFormat();
    cf.setForeground(color);
    mLog->setCurrentCharFormat(cf);
    mLog->appendPlainText(text);
    if (!context.isEmpty()) {
        cf.setForeground(Qt::gray);
        mLog->setCurrentCharFormat(cf);
        mLog->insertPlainText(context);
    }
}

void MainWindow::log(const QString &text, QColor color)
{
    log(text, QString(), color);
}

bool MainWindow::createPkiPath(const QString &path)
{
    const QString msg = tr("Creating PKI path '%1': %2");

    QDir dir;
    const bool ret = dir.mkpath(path);
    if (ret) {
        qDebug() << msg.arg(path).arg("SUCCESS.");
    } else {
        qCritical(msg.arg(path).arg("FAILED.").toLocal8Bit());
    }

    return ret;
}

bool MainWindow::createPkiFolders()
{
    bool result = createPkiPath(m_pkiConfig.trustListDirectory());
    if (!result)
        return result;

    result = createPkiPath(m_pkiConfig.revocationListDirectory());
    if (!result)
        return result;

    result = createPkiPath(m_pkiConfig.issuerListDirectory());
    if (!result)
        return result;

    result = createPkiPath(m_pkiConfig.issuerRevocationListDirectory());
    if (!result)
        return result;

    return result;
}

void MainWindow::showErrorDialog(QOpcUaErrorState *errorState)
{
    QString msg;
    CertificateDialog dlg;
    int result = 0;

    const QString statuscode = QOpcUa::statusToString(errorState->errorCode());

    if (errorState->isClientSideError())
        msg = tr("The client reported: ");
    else
        msg = tr("The server reported: ");

    switch (errorState->connectionStep()) {
    case QOpcUaErrorState::ConnectionStep::CertificateValidation:
        msg += tr("Server certificate validation failed with error 0x%1 (%2).\nClick 'Abort' to abort the connect, or 'Ignore' to continue connecting.")
                  .arg(static_cast<ulong>(errorState->errorCode()), 8, 16, QLatin1Char('0')).arg(statuscode);
        result = dlg.showCertificate(msg, m_endpoint.serverCertificate(), m_pkiConfig.trustListDirectory());
        errorState->setIgnoreError(result == 1);
        break;
    case QOpcUaErrorState::ConnectionStep::OpenSecureChannel:
        msg += tr("OpenSecureChannel failed with error 0x%1 (%2).").arg(errorState->errorCode(), 8, 16, QLatin1Char('0')).arg(statuscode);
        QMessageBox::warning(this, tr("Connection Error"), msg);
        break;
    case QOpcUaErrorState::ConnectionStep::CreateSession:
        msg += tr("CreateSession failed with error 0x%1 (%2).").arg(errorState->errorCode(), 8, 16, QLatin1Char('0')).arg(statuscode);
        QMessageBox::warning(this, tr("Connection Error"), msg);
        break;
    case QOpcUaErrorState::ConnectionStep::ActivateSession:
        msg += tr("ActivateSession failed with error 0x%1 (%2).").arg(errorState->errorCode(), 8, 16, QLatin1Char('0')).arg(statuscode);
        QMessageBox::warning(this, tr("Connection Error"), msg);
        break;
    }
}
