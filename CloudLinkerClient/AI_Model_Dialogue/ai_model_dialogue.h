#ifndef AI_MODEL_DIALOGUE_H
#define AI_MODEL_DIALOGUE_H

#include <QWidget>
#include<QNetworkAccessManager>

namespace Ui {
class AI_Model_Dialogue;
}

class AI_Model_Dialogue : public QWidget
{
    Q_OBJECT

public:
    explicit AI_Model_Dialogue(QWidget *parent = nullptr);
    ~AI_Model_Dialogue();

private slots:
    void on_send_pushButton_clicked();

private:
    Ui::AI_Model_Dialogue *ui;
    QNetworkAccessManager* manager;
};

#endif // AI_MODEL_DIALOGUE_H
