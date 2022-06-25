#include "qform1.h"
#include "ui_qform1.h"

QForm1::QForm1(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QForm1)
{
    ui->setupUi(this);

    QString currDir, strDir;
    QDir dir;

    currDir = qApp->applicationDirPath();

    strDir = currDir + "/mbed";
    if(dir.exists(strDir))
        ui->lineEdit->setText(strDir);

    strDir = currDir + "/gcc-arm-none-eabi-6-2017-q2-update-win32/bin";
    if(dir.exists(strDir))
        ui->lineEdit_2->setText(strDir);

    strDir = currDir + "/Make/bin";
    if(dir.exists(strDir))
        ui->lineEdit_3->setText(strDir);

    strDir = currDir + "/MyMake";
    if(dir.exists(strDir))
        ui->lineEdit_4->setText(strDir);

}

QForm1::~QForm1()
{
    delete ui;
}


void QForm1::on_pushButton_clicked()
{
    QString str;

    str = QFileDialog::getExistingDirectory(this, "MBED DIR");
    if(str != "")
        ui->lineEdit->setText(str);
}


void QForm1::on_pushButton_2_clicked()
{
    QString str;

    str = QFileDialog::getExistingDirectory(this, "COMPILER BIN DIR");
    if(str != "")
        ui->lineEdit_2->setText(str);
}


void QForm1::on_pushButton_5_clicked()
{
    QString str;

    str = QFileDialog::getExistingDirectory(this, "MAKE FOLDER");
    if(str != "")
        ui->lineEdit_3->setText(str);
}


void QForm1::on_pushButton_6_clicked()
{
    QString str;

    str = QFileDialog::getExistingDirectory(this, "MyMake FOLDER");
    if(str != "")
        ui->lineEdit_4->setText(str);
}


void QForm1::on_pushButton_3_clicked()
{
    FILE *myMakeCFG, *myJsonC_CPP_Properties, *myJsonLaunch, *myJsonTasks;
    FILE *myJsonC_CPP_PropertiesBase, *myJsonLaunchBase, *myJsonTasksBase;
    QDir dir;

    QString currdir, myMakeCFGDir;
    QString fileName;
    char c, strAux[1024];
    int i;

    if(ui->lineEdit->text() == ""){
        QMessageBox::information(this, "VSCConfig", "Select a MBED DIR");
        return;
    }

    if(ui->lineEdit_2->text() == ""){
        QMessageBox::information(this, "VSCConfig", "Select a COMPILER DIR");
        return;
    }

    if(ui->lineEdit_3->text() == ""){
        QMessageBox::information(this, "VSCConfig", "Select a MAKE BIN DIR");
        return;
    }

    if(ui->lineEdit_4->text() == ""){
        QMessageBox::information(this, "VSCConfig", "Select a MyMAKE DIR");
        return;
    }


    if(!dir.exists(ui->lineEdit->text())){
        QMessageBox::information(this, "VSCConfig", "MBED DIR Not FOUND");
        return;
    }

    if(!dir.exists(ui->lineEdit_2->text())){
        QMessageBox::information(this, "VSCConfig", "COMPILER BIN DIR Not FOUND");
        return;
    }

    if(!dir.exists(ui->lineEdit_3->text())){
        QMessageBox::information(this, "VSCConfig", "MAKE DIR Not FOUND");
        return;
    }

    if(!dir.exists(ui->lineEdit_4->text())){
        QMessageBox::information(this, "VSCConfig", "MyMAKE DIR Not FOUND");
        return;
    }


    currdir = qApp->applicationDirPath();

    if(!dir.exists(currdir + "/VSCBaseProject/.vscode")){
        QMessageBox::information(this, "VSCConfig", ".vscode DIR Not FOUND");
        return;
    }

    myMakeCFGDir = currdir + "/VSCBaseProject/myMakeCFG";

    if ((myMakeCFG = fopen(myMakeCFGDir.toUtf8().data(), "wt"))== NULL){
        QMessageBox::information(this, "VSCConfig", "Cannot open myMakeCFG file.");
        return;
    }


    fprintf(myMakeCFG, "%s\n", ui->lineEdit_2->text().toUtf8().data());
    fprintf(myMakeCFG, "%s\n", ui->lineEdit->text().toUtf8().data());
    fprintf(myMakeCFG, "%s\n", ui->lineEdit_3->text().toUtf8().data());

    fclose(myMakeCFG);


    fileName = currdir + "/json_c_cpp_properties";

    if ((myJsonC_CPP_PropertiesBase = fopen(fileName.toUtf8().data(), "rt"))== NULL){
        QMessageBox::information(this, "VSCConfig", "Cannot open BASE C_CPP_Properties file.");
        return;
    }

    fileName = currdir + "/VSCBaseProject/.vscode/c_cpp_properties.json";

    if ((myJsonC_CPP_Properties = fopen(fileName.toUtf8().data(), "wt"))== NULL){
        QMessageBox::information(this, "VSCConfig", "Cannot open myJsonC_CPP_Properties file.");
        return;
    }

    i = 0;
    while((c = fgetc(myJsonC_CPP_PropertiesBase)) != EOF){
        strAux[i] = c;
        switch(i){
        case 0:
            if(c == '&')
                i = 1;
            else
                fputc(c, myJsonC_CPP_Properties);
            break;
        case 1:
            if(c == '&')
                i = 2;
            else{
                strAux[++i] = '\0';
                fprintf(myJsonC_CPP_Properties, strAux);
                i = 0;
            }
            break;
        case 2:
            switch(c){
            case '0':
                strncpy(strAux, ui->lineEdit_2->text().toUtf8().data(), ui->lineEdit_2->text().length());
                strAux[ui->lineEdit_2->text().length()] = '\0';
                strncat(strAux, "/arm-none-eabi-g++", 19);
                break;
            case '1':
                strcpy(strAux, "");
                strncat(strAux, ui->lineEdit->text().toUtf8().data(), ui->lineEdit->text().length());
                break;
            default:
                strAux[++i] = '\0';
            }
            fprintf(myJsonC_CPP_Properties, strAux);
            i = 0;
            break;
        }
    }

    fclose(myJsonC_CPP_PropertiesBase);
    fclose(myJsonC_CPP_Properties);

    fileName = currdir + "/json_launch";
    if ((myJsonLaunchBase = fopen(fileName.toUtf8().data(), "rt"))== NULL){
        QMessageBox::information(this, "VSCConfig", "Cannot open BASE Lunch file.");
        return;
    }

    fileName = currdir + "/VSCBaseProject/.vscode/launch.json";
    if ((myJsonLaunch = fopen(fileName.toUtf8().data(), "wt"))== NULL){
        QMessageBox::information(this, "VSCConfig", "Cannot open myJsonLaunch file.");
        return;
    }

    i = 0;
    while((c = fgetc(myJsonLaunchBase)) != EOF){
        strAux[i] = c;
        switch(i){
        case 0:
            if(c == '&')
                i = 1;
            else
                fputc(c, myJsonLaunch);
            break;
        case 1:
            if(c == '&')
                i = 2;
            else{
                strAux[++i] = '\0';
                fprintf(myJsonLaunch, strAux);
                i = 0;
            }
            break;
        case 2:
            switch(c){
            case '0':
                strncpy(strAux, ui->lineEdit_4->text().toUtf8().data(), ui->lineEdit_4->text().length());
                strAux[ui->lineEdit_4->text().length()] = '\0';
#ifdef WIN32
                strncat(strAux, "/MyMake.exe", 12);
#else
                strncat(strAux, "/MyMake", 8);
#endif
                break;
            case '1':
#ifdef WIN32
                strncpy(strAux, "cppvsdbg", 9);
#else
                strncpy(strAux, "cppdbg", 7);
#endif
                break;
            default:
                strAux[++i] = '\0';
            }
            fprintf(myJsonLaunch, strAux);
            i = 0;
            break;
        }
    }

    fclose(myJsonLaunchBase);
    fclose(myJsonLaunch);

    fileName = currdir + "/json_tasks";
    if ((myJsonTasksBase = fopen(fileName.toUtf8().data(), "rt"))== NULL){
        QMessageBox::information(this, "VSCConfig", "Cannot open BASE Tasks file.");
        return;
    }

    fileName = currdir + "/VSCBaseProject/.vscode/tasks.json";
    if ((myJsonTasks = fopen(fileName.toUtf8().data(), "wt"))== NULL){
        QMessageBox::information(this, "VSCConfig", "Cannot open myJsonTasks file.");
        return;
    }

    i = 0;
    while((c = fgetc(myJsonTasksBase)) != EOF){
        strAux[i] = c;
        switch(i){
        case 0:
            if(c == '&')
                i = 1;
            else
                fputc(c, myJsonTasks);
            break;
        case 1:
            if(c == '&')
                i = 2;
            else{
                strAux[++i] = '\0';
                fprintf(myJsonTasks, strAux);
                i = 0;
            }
            break;
        case 2:
            switch(c){
            case '0':
                strncpy(strAux, ui->lineEdit->text().toUtf8().data(), ui->lineEdit->text().length());
                strAux[ui->lineEdit->text().length()] = '\0';
                break;
            case '1':
                strncpy(strAux, ui->lineEdit_4->text().toUtf8().data(), ui->lineEdit_4->text().length());
                strAux[ui->lineEdit_4->text().length()] = '\0';
#ifdef WIN32
                strncat(strAux, "/MyMake.exe", 12);
#else
                strncat(strAux, "/MyMake", 8);
#endif
                break;
            default:
                strAux[++i] = '\0';
            }
            fprintf(myJsonLaunch, strAux);
            i = 0;
            break;
        }
    }

    fclose(myJsonTasksBase);
    fclose(myJsonTasks);


    QMessageBox::information(this, "VSCConfig", "Configuration FINISHED Ok");

}


void QForm1::on_pushButton_4_clicked()
{
    close();
}

