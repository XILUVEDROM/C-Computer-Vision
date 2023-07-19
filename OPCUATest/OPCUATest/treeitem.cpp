#include "treeitem.h"
#include "opcuamodel.h"
#include <QOpcUaArgument>
#include <QOpcUaAxisInformation>
#include <QOpcUaClient>
#include <QOpcUaComplexNumber>
#include <QOpcUaDoubleComplexNumber>
#include <QOpcUaEUInformation>
#include <QOpcUaExtensionObject>
#include <QOpcUaLocalizedText>
#include <QOpcUaQualifiedName>
#include <QOpcUaRange>
#include <QOpcUaXValue>
#include <QMetaEnum>
#include <QPixmap>

const int numberOfDisplayColumns = 7; // NodeId, Value, NodeClass, DataType, BrowseName, DisplayName, Description

TreeItem::TreeItem(OpcUaModel *model) : QObject(nullptr)
  , mModel(model)
{
}

TreeItem::TreeItem(QOpcUaNode *node, OpcUaModel *model, TreeItem *parent) : QObject(parent)
  , mOpcNode(node)
  , mModel(model)
  , mParentItem(parent)
{
    connect(mOpcNode.get(), &QOpcUaNode::attributeRead, this, &TreeItem::handleAttributes);
    connect(mOpcNode.get(), &QOpcUaNode::browseFinished, this, &TreeItem::browseFinished);

    if (!mOpcNode->readAttributes( QOpcUa::NodeAttribute::Value
                            | QOpcUa::NodeAttribute::NodeClass
                            | QOpcUa::NodeAttribute::Description
                            | QOpcUa::NodeAttribute::DataType
                            | QOpcUa::NodeAttribute::BrowseName
                            | QOpcUa::NodeAttribute::DisplayName
                            ))
        qWarning() << "Reading attributes" << mOpcNode->nodeId() << "failed";
}

TreeItem::TreeItem(QOpcUaNode *node, OpcUaModel *model, const QOpcUaReferenceDescription &browsingData, TreeItem *parent) : TreeItem(node, model, parent)
{
    mNodeBrowseName = browsingData.browseName().name();
    mNodeClass = browsingData.nodeClass();
    mNodeId = browsingData.targetNodeId().nodeId();
    mNodeDisplayName = browsingData.displayName().text();
}

TreeItem::~TreeItem()
{
    qDeleteAll(mChildItems);
}

TreeItem *TreeItem::child(int row)
{
    if (row >= mChildItems.size())
        qCritical() << "TreeItem in row" << row << "does not exist.";
    return mChildItems[row];
}

int TreeItem::childIndex(const TreeItem *child) const
{
    return mChildItems.indexOf(const_cast<TreeItem *>(child));
}

int TreeItem::childCount()
{
    startBrowsing();
    return mChildItems.size();
}

int TreeItem::columnCount() const
{
    return numberOfDisplayColumns;
}

QVariant TreeItem::data(int column)
{
    if (column == 0) {
        return mNodeBrowseName;
    } else if (column == 1) {
        if (!mAttributesReady)
            return tr("Loading ...");

        const auto type = mOpcNode->attribute(QOpcUa::NodeAttribute::DataType).toString();
        const auto value = mOpcNode->attribute(QOpcUa::NodeAttribute::Value);

        return variantToString(value, type);
    } else if (column == 2) {
        QMetaEnum metaEnum = QMetaEnum::fromType<QOpcUa::NodeClass>();
        QString name = metaEnum.valueToKey((uint)mNodeClass);
        return name + " (" + QString::number((uint)mNodeClass) + ")";
    } else if (column == 3) {
        if (!mAttributesReady)
            return tr("Loading ...");

        const QString typeId = mOpcNode->attribute(QOpcUa::NodeAttribute::DataType).toString();
        auto enumEntry = QOpcUa::namespace0IdFromNodeId(typeId);
        QString name;
        if (enumEntry == QOpcUa::NodeIds::Namespace0::Unknown)
            return typeId;
        return QOpcUa::namespace0IdName(enumEntry) + " (" + typeId + ")";
    } else if (column == 4) {
        return mNodeId;
    } else if (column == 5) {
        return mNodeDisplayName;
    } else if (column == 6) {
        if (!mAttributesReady)
            return tr("Loading ...");

        return mOpcNode->attribute(QOpcUa::NodeAttribute::Description).value<QOpcUaLocalizedText>().text();
    }
    return QVariant();
}

int TreeItem::row() const
{
    if (!mParentItem)
        return 0;
    return mParentItem->childIndex(this);
}

TreeItem *TreeItem::parentItem()
{
    return mParentItem;
}

void TreeItem::appendChild(TreeItem *child)
{
    if (!child)
        return;

    if (!hasChildNodeItem(child->mNodeId)) {
        mChildItems.append(child);
        mChildNodeIds.insert(child->mNodeId);
    } else {
        child->deleteLater();
    }
}

QPixmap TreeItem::icon(int column) const
{
    if (column != 0 || !mOpcNode)
        return QPixmap();

    QColor c;

    switch (mNodeClass) {
    case QOpcUa::NodeClass::Object:
        c = Qt::darkGreen;
        break;
    case QOpcUa::NodeClass::Variable:
        c = Qt::darkBlue;
        break;
    case QOpcUa::NodeClass::Method:
        c = Qt::darkRed;
        break;
    default:
        c = Qt::gray;
    }

    QPixmap p(10,10);
    p.fill(c);
    return p;
}

bool TreeItem::hasChildNodeItem(const QString &nodeId) const
{
    return mChildNodeIds.contains(nodeId);
}

void TreeItem::startBrowsing()
{
    if (mBrowseStarted)
        return;

    if (!mOpcNode->browseChildren())
        qWarning() << "Browsing node" << mOpcNode->nodeId() << "failed";
    else
        mBrowseStarted = true;
}

void TreeItem::handleAttributes(QOpcUa::NodeAttributes attr)
{
    if (attr & QOpcUa::NodeAttribute::NodeClass)
        mNodeClass = mOpcNode->attribute(QOpcUa::NodeAttribute::NodeClass).value<QOpcUa::NodeClass>();
    if (attr & QOpcUa::NodeAttribute::BrowseName)
        mNodeBrowseName = mOpcNode->attribute(QOpcUa::NodeAttribute::BrowseName).value<QOpcUaQualifiedName>().name();
    if (attr & QOpcUa::NodeAttribute::DisplayName)
        mNodeDisplayName = mOpcNode->attribute(QOpcUa::NodeAttribute::DisplayName).value<QOpcUaLocalizedText>().text();

    mAttributesReady = true;
    emit mModel->dataChanged(mModel->createIndex(row(), 0, this), mModel->createIndex(row(), numberOfDisplayColumns - 1, this));
}

void TreeItem::browseFinished(QVector<QOpcUaReferenceDescription> children, QOpcUa::UaStatusCode statusCode)
{
    if (statusCode != QOpcUa::Good) {
        qWarning() << "Browsing node" << mOpcNode->nodeId() << "finally failed:" << statusCode;
        return;
    }

    auto index = mModel->createIndex(row(), 0, this);

    for (const auto &item : children) {
        if (hasChildNodeItem(item.targetNodeId().nodeId()))
            continue;

        auto node = mModel->opcUaClient()->node(item.targetNodeId());
        if (!node) {
            qWarning() << "Failed to instantiate node:" << item.targetNodeId().nodeId();
            continue;
        }

        mModel->beginInsertRows(index, mChildItems.size(), mChildItems.size() + 1);
        appendChild(new TreeItem(node, mModel, item, this));
        mModel->endInsertRows();
    }

    emit mModel->dataChanged(mModel->createIndex(row(), 0, this), mModel->createIndex(row(), numberOfDisplayColumns - 1, this));
}

QString TreeItem::variantToString(const QVariant &value, const QString &typeNodeId) const
{
    if (value.type() == QVariant::List) {
        const auto list = value.toList();
        QStringList concat;

        for (auto it : list)
            concat.append(variantToString(it, typeNodeId));

        return concat.join("\n");
    }

    if (typeNodeId == QLatin1String("ns=0;i=19")) { // StatusCode
        const char *name = QMetaEnum::fromType<QOpcUa::UaStatusCode>().valueToKey(value.toInt());
        if (!name)
            return QLatin1String("Unknown StatusCode");
        else
            return QString(name);
    }
    if (typeNodeId == QLatin1String("ns=0;i=2")) // Char
        return QString::number(value.toInt());
    else if (typeNodeId == QLatin1String("ns=0;i=3")) // SChar
        return QString::number(value.toUInt());
    else if (typeNodeId == QLatin1String("ns=0;i=4")) // Int16
        return QString::number(value.toInt());
    else if (typeNodeId == QLatin1String("ns=0;i=5")) // UInt16
        return QString::number(value.toUInt());
    else if (value.type() == QVariant::ByteArray)
        return QLatin1String("0x") + value.toByteArray().toHex();
    else if (value.type() == QVariant::DateTime)
        return value.toDateTime().toString(Qt::ISODate);
    else if (value.canConvert<QOpcUaQualifiedName>()) {
        const auto name = value.value<QOpcUaQualifiedName>();
        return QStringLiteral("[NamespaceIndex: %1, Name: \"%2\"]").arg(name.namespaceIndex()).arg(name.name());
    } else if (value.canConvert<QOpcUaLocalizedText>()) {
        const auto text = value.value<QOpcUaLocalizedText>();
        return localizedTextToString(text);
    } else if (value.canConvert<QOpcUaRange>()) {
        const auto range = value.value<QOpcUaRange>();
        return rangeToString(range);
    } else if (value.canConvert<QOpcUaComplexNumber>()) {
        const auto complex = value.value<QOpcUaComplexNumber>();
        return QStringLiteral("[Real: %1, Imaginary: %2]").arg(complex.real()).arg(complex.imaginary());
    } else if (value.canConvert<QOpcUaDoubleComplexNumber>()) {
        const auto complex = value.value<QOpcUaDoubleComplexNumber>();
        return QStringLiteral("[Real: %1, Imaginary: %2]").arg(complex.real()).arg(complex.imaginary());
    } else if (value.canConvert<QOpcUaXValue>()) {
        const auto xv = value.value<QOpcUaXValue>();
        return QStringLiteral("[X: %1, Value: %2]").arg(xv.x()).arg(xv.value());
    } else if (value.canConvert<QOpcUaEUInformation>()) {
        const auto info = value.value<QOpcUaEUInformation>();
        return euInformationToString(info);
    } else if (value.canConvert<QOpcUaAxisInformation>()) {
        const auto info = value.value<QOpcUaAxisInformation>();
        return QStringLiteral("[EUInformation: %1, EURange: %2, Title: %3 , AxisScaleType: %4, AxisSteps: %5]").arg(
                    euInformationToString(info.engineeringUnits())).arg(rangeToString(info.eURange())).arg(localizedTextToString(info.title())).arg(
                        info.axisScaleType() == QOpcUa::AxisScale::Linear ? "Linear" : (info.axisScaleType() == QOpcUa::AxisScale::Ln) ? "Ln" : "Log").arg(
                        numberArrayToString(info.axisSteps()));
    } else if (value.canConvert<QOpcUaExpandedNodeId>()) {
        const auto id = value.value<QOpcUaExpandedNodeId>();
        return QStringLiteral("[NodeId: \"%1\", ServerIndex: \"%2\", NamespaceUri: \"%3\"]").arg(
                    id.nodeId()).arg(id.serverIndex()).arg(id.namespaceUri());
    } else if (value.canConvert<QOpcUaArgument>()) {
        const auto a = value.value<QOpcUaArgument>();

        return QStringLiteral("[Name: \"%1\", DataType: \"%2\", ValueRank: \"%3\", ArrayDimensions: %4, Description: %5]").arg(
                    a.name()).arg(a.dataTypeId()).arg(a.valueRank()).arg(numberArrayToString(a.arrayDimensions())).arg(
                    localizedTextToString(a.description()));
    } else if (value.canConvert<QOpcUaExtensionObject>()) {
        const auto obj = value.value<QOpcUaExtensionObject>();
        return QStringLiteral("[TypeId: \"%1\", Encoding: %2, Body: 0x%3]").arg(obj.encodingTypeId()).arg(
                    obj.encoding() == QOpcUaExtensionObject::Encoding::NoBody ?
                        "NoBody" : (obj.encoding() == QOpcUaExtensionObject::Encoding::ByteString ?
                            "ByteString" : "XML")).arg(obj.encodedBody().isEmpty() ? "0" : QString(obj.encodedBody().toHex()));
    }

    if (value.canConvert<QString>())
        return value.toString();

    return QString();
}

QString TreeItem::localizedTextToString(const QOpcUaLocalizedText &text) const
{
    return QStringLiteral("[Locale: \"%1\", Text: \"%2\"]").arg(text.locale()).arg(text.text());
}

QString TreeItem::rangeToString(const QOpcUaRange &range) const
{
    return QStringLiteral("[Low: %1, High: %2]").arg(range.low()).arg(range.high());
}

QString TreeItem::euInformationToString(const QOpcUaEUInformation &info) const
{
    return QStringLiteral("[UnitId: %1, NamespaceUri: \"%2\", DisplayName: %3, Description: %4]").arg(info.unitId()).arg(
                info.namespaceUri()).arg(localizedTextToString(info.displayName())).arg(localizedTextToString(info.description()));
}
