#include "T_TableViewModel.h"


T_TableViewModel::T_TableViewModel(QObject* parent)
    : QAbstractTableModel{parent}
{
    _header << "" << "文件名" << "大小" << "类型";
    // _Data.append({"玫瑰少年", "23.MB", "mp3", QIcon(":/Resource/Image/Model/MaVieEnRose.jpg")});
    // addView("玫瑰少年","23.MB","mp3",QIcon(":/Resource/Image/Model/MaVieEnRose.jpg"));

}

T_TableViewModel::~T_TableViewModel()
{

}

int T_TableViewModel::rowCount(const QModelIndex& parent) const
{
    return _Data.size();
}

int T_TableViewModel::columnCount(const QModelIndex& parent) const
{
    return _header.count();
}

void T_TableViewModel::addView(QString& name, QString& fileSize, QString& fileType, QIcon& icon)
{
    beginInsertRows(QModelIndex(), _Data.size(), _Data.size());
    _Data.append({name,fileSize,fileType,icon});
    endInsertRows();
}

T_TableViewModel &T_TableViewModel::getInstance()
{
    static T_TableViewModel instance;
    return instance;
}

void T_TableViewModel::clearView()
{
    beginResetModel();
    _Data.clear();
    endResetModel();
}

QString T_TableViewModel::getFileName(int row)
{
    if (row >= 0 && row < _Data.size()) {
        return _Data[row].name;
    }
    return QString();
}

QString T_TableViewModel::getFiletype(int row)
{
    if (row >= 0 && row < _Data.size()) {
        return _Data[row].type;
    }
    return QString();
}

QVariant T_TableViewModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= _Data.size())
        return QVariant();

    const int row = index.row();
    const int col = index.column();

    // 图标列
    if (col == 0 && role == Qt::DecorationRole) {
        return _Data[row].icon;
    }

    // 文本列
    if (col != 0) {
        if (role == Qt::DisplayRole) {
            switch (col) {
            case 1: return _Data[row].name;    // 文件名
            case 2: return _Data[row].type;  // 类型
            case 3: return _Data[row].size; // 大小
            }
        }
        // 添加文本对齐控制 ▼▼▼▼
        if (role == Qt::TextAlignmentRole) {
            if (col == 3)
            { // 大小列居中
                return Qt::AlignCenter;
            }
            else if(col==2)
            {
                return Qt::AlignCenter;
            }
        }
        // ▲▲▲▲
    }

    return QVariant();
}

QVariant T_TableViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            return _header[section];
        }
        // 增加对齐设置代码 ▼▼▼▼
        if (role == Qt::TextAlignmentRole) {
            // 设置第1列（索引0）居中，其他列左对齐
            if (section == 0)
            {
                return Qt::AlignCenter;
            }
            else if(section == 1)
            {
                return Qt::AlignLeft || Qt::AlignVCenter;
            }
            else
            {
                return Qt::AlignCenter;
            }
        }
        // ▲▲▲▲
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}
