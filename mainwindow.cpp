#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "TransformCoordinate.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList lstCoor;
    lstCoor << "WGS84" << "GCJ02" << "BD09LL" << "BD09MC";
    ui->cbSrcCoor->addItems(lstCoor);
    ui->cbSrcCoor->setCurrentIndex(WGS84);
    ui->cbDstCoor->addItems(lstCoor);
    ui->cbDstCoor->setCurrentIndex(GCJ02);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbConversion_clicked()
{
    double x0, y0, x1, y1;
    x0 = ui->leSrcLong->text().toDouble();
    y0 = ui->leSrcLat->text().toDouble();
    
    int nRet = TransformCoordinate(x0, y0,
                          x1, y1, 
                          (_COORDINATE)ui->cbSrcCoor->currentIndex(),
                          (_COORDINATE)ui->cbDstCoor->currentIndex());
    if(nRet)
        return;
    
    ui->leDstLong->setText(QString::number(x1));
    ui->leDstLat->setText(QString::number(y1));
}

void MainWindow::on_pbBrowsSrcFile_clicked()
{
    QString szFile = QFileDialog::getOpenFileName(this, tr("Open source file"), 
                                                  QString(),
                                                  tr("GPX file(*.gpx);;NMea file(*.nmea);;ACT file(*.act);;txt(*.txt);;All files(*.*)"));
    ui->leSrcFile->setText(szFile);
}

void MainWindow::on_pbBrowsDstFile_clicked()
{
    QString szFile = QFileDialog::getSaveFileName(this, tr("Open Destination file"),
                                                  QString(),
                                                  tr("GPX file(*.gpx);;"));
    ui->leDstFile->setText(szFile);
}

void MainWindow::on_pbConversionFile_clicked()
{
#ifdef BUILD_GPXMODEL
    TransformCoordinateFiles(ui->leSrcFile->text().toStdString().c_str(),
                    ui->leDstFile->text().toStdString().c_str(),
                    (_COORDINATE)ui->cbSrcCoor->currentIndex(),
                    (_COORDINATE)ui->cbDstCoor->currentIndex());
    QMessageBox::information(this, tr("End"), tr("Transform coordinate end"));
#else
    qDebug() << "Please set BUILD_GPXMODEL to ON";
#endif
}
