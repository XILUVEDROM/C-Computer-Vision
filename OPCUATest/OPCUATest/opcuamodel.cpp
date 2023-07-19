#include "opcuamodel.h"
#include "treeitem.h"
#include <QOpcUaClient>
#include <QOpcUaNode>
#include <QIcon>

OpcUaModel::OpcUaModel(QObject *parent) : QAbstractItemModel(parent)
{
}

void OpcUaModel::setOpcUaClient(QOpcUaClient *client)
{
    beginResetModel();
    mOpcUaClient = client;
    if (mOpcUaClient)
        mRootItem.reset(new TreeItem(client->node("ns=0;i=84"), this /* model */, nullptr /* parent */));
    else
        mRootItem.reset(nullptr);
    endResetModel();
}

QOpcUaClient *OpcUaModel::opcUaClient() const
{
    return mOpcUaClient;
}

QVariant OpcUaModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto item = static_cast<TreeItem *>(index.internalPointer());

    if (role == Qt::DisplayRole) {
        return item->data(index.column());
    } else if (role ==  Qt::DecorationRole && index.column() == 0) {
        return item->icon(index.column());
    }

    return QVariant();
}

QVariant OpcUaModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        if (section == 0)
            return QString("BrowseName");
        else if (section == 1)
            return QString("Value");
        else if (section == 2)
            return QString("NodeClass");
        else if (section == 3)
            return QString("DataType");
        else if (section == 4)
            return QString("NodeId");
        else if (section == 5)
            return QString("DisplayName");
        else if (section == 6)
            return QString("Description");
        else
            return QString("Column %1").arg(section);
    }
    else
        return QString("Row %1").arg(section);
}

QModelIndex OpcUaModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *item = nullptr;

    if (!parent.isValid()) {
        item = mRootItem.get();
    } else {
        item = static_cast<TreeItem*>(parent.internalPointer())->child(row);
    }

    if (item)
        return createIndex(row, column, item);
    else
        return QModelIndex();
}

QModelIndex OpcUaModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    auto childItem = static_cast<TreeItem*>(index.internalPointer());
    auto parentItem = childItem->parentItem();

    if (childItem == mRootItem.get() || !parentItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int OpcUaModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (!mOpcUaClient)
        return 0;

    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        return 1; // only one root item
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    if (!parentItem)
        return 0;

    return parentItem->childCount();
}

int OpcUaModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else if (mRootItem)
        return mRootItem->columnCount();
    else
        return 0;
}
