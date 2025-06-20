#ifndef T_TABLEVIEWMODEL_H
#define T_TABLEVIEWMODEL_H

#include <QAbstractTableModel>
#include <QIcon>

class T_TableViewModel : public QAbstractTableModel
{
    Q_OBJECT
    struct Info {
        QString name;
        QString size;
        QString type;
        QIcon icon;
    };
public:
    explicit T_TableViewModel(QObject* parent = nullptr);
    ~T_TableViewModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    void addView(QString& name, QString& fileSize, QString& fileType, QIcon& icon);
    static T_TableViewModel&getInstance();
    void clearView();
    QString getFileName(int row);
    QString getFiletype(int row);
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
private:
    QStringList _header;
    QVector<Info> _Data;
};

#endif // T_TABLEVIEWMODEL_H
