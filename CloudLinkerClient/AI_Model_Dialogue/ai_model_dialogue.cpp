#include "ai_model_dialogue.h"
#include "ui_ai_model_dialogue.h"

#include<QNetworkRequest>
#include<QJsonArray>
#include<QJsonObject>
#include<QJsonDocument>
#include<QJsonParseError>
#include<QNetworkReply>

AI_Model_Dialogue::AI_Model_Dialogue(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AI_Model_Dialogue)
{
    ui->setupUi(this);
    manager=new QNetworkAccessManager;
}

AI_Model_Dialogue::~AI_Model_Dialogue()
{
    delete ui;
}

void AI_Model_Dialogue::on_send_pushButton_clicked()
{
    QString text=ui->input_textEdit->toPlainText();

    ui->input_textEdit->clear();
    ui->output_textEdit->insertPlainText("-->");
    ui->output_textEdit->insertPlainText(text);
    ui->output_textEdit->insertPlainText("\n\n");

    QNetworkRequest request;
    request.setUrl(QUrl("https://api.deepseek.com/chat/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("Accept","application/json");
    request.setRawHeader("Authorization","Bearer sk-bb4f1b994f294edb9d92e4489eba22bd");

    QJsonObject requestBody;

    QJsonArray messages;

    QJsonObject systemMsg;
    systemMsg["content"]="You are a helpful assistant";
    systemMsg["role"]="system";

    QJsonObject userMsg;
    userMsg["role"]="user";
    userMsg["content"]=text;

    messages.append(systemMsg);
    messages.append(userMsg);


    requestBody["messages"]=messages;
    requestBody["model"]="deepseek-chat";
    requestBody["max_tokens"]=2048;
    requestBody["stream"]=true;
    requestBody["temperature"]=1;

    QNetworkReply*reply=manager->post(request,QJsonDocument(requestBody).toJson());

    connect(reply,&QNetworkReply::readyRead,this,[=]{
        while(reply->canReadLine())
        {
            QString line=reply->readLine().trimmed();

            if(line.startsWith("data: "))
            {
                line.remove(0,6);

                QJsonParseError error;
                QJsonDocument doc=QJsonDocument::fromJson(line.toUtf8(),&error);
                if(error.error==QJsonParseError::NoError)
                {
                    QString content=doc.object()["choices"].toArray().first().toObject()["delta"].toObject()["content"].toString();
                    if(!content.isEmpty())
                    {
                        qDebug()<<content;
                        ui->output_textEdit->moveCursor(QTextCursor::End);
                        ui->output_textEdit->insertPlainText(content);
                    }
                }
            }
        }
    });
    connect(reply,&QNetworkReply::finished,this,[=]{
        ui->output_textEdit->moveCursor(QTextCursor::End);
        ui->output_textEdit->insertPlainText("\n\n\n\n");
        reply->deleteLater();
    });
}

