#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "TransformCoordinate.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QCursor>
#include <QLocale>
#include <QStatusBar>

#ifdef RABBITCOMMON
    #include "DlgAbout/DlgAbout.h"
    #include "FrmUpdater/FrmUpdater.h"
    #include "RabbitCommonDir.h"
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);    

    CFrmUpdater updater;
    ui->actionUpdate_U->setIcon(updater.windowIcon());
    
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
                      static_cast<_COORDINATE>(ui->cbSrcCoor->currentIndex()),
                      static_cast<_COORDINATE>(ui->cbDstCoor->currentIndex()));
    if(nRet)
    {
        SetStatusInfo(tr("Conver fail"), Qt::red);
        return;
    }
    
    ui->leDstLong->setText(QString::number(x1));
    ui->leDstLat->setText(QString::number(y1));
}

void MainWindow::on_pbBrowsSrcFile_clicked()
{
    QString szExt = tr("GPX file(*.gpx);;NMea file(*.nmea);;ACT file(*.act);;txt(*.txt);;All files(*.*)");
    QString szFile = RabbitCommon::CDir::GetOpenFileName(this, 
                                        tr("Open source file"),
                                        QString(), szExt);
    if(szFile.isEmpty()) return;
    ui->leSrcFile->setText(szFile);
}

void MainWindow::on_pbBrowsDstFile_clicked()
{
    QString szExt = tr("GPX file(*.gpx);;");
#ifdef BUILD_LIBKML
	szExt += tr("KML file(*.kml)");
#endif
    QString szFile = RabbitCommon::CDir::GetOpenFileName(this, 
                                  tr("Open destination file"),
                                  QString(), szExt);
    if(szFile.isEmpty()) return;
    ui->leDstFile->setText(szFile);
}

void MainWindow::on_pbConversionFile_clicked()
{
#ifdef BUILD_GPXMODEL
    this->statusBar()->showMessage(tr("Start transform ......"));
    QCursor cursor = this->cursor();
    this->setCursor(Qt::WaitCursor);
    TransformCoordinateFiles(ui->leSrcFile->text().toStdString().c_str(),
                    ui->leDstFile->text().toStdString().c_str(),
                    (_COORDINATE)ui->cbSrcCoor->currentIndex(),
                    (_COORDINATE)ui->cbDstCoor->currentIndex());
    //QMessageBox::information(this, tr("End"), tr("Transform coordinate end"));
    this->setCursor(cursor);
    this->statusBar()->showMessage(tr("Ready"));
#else
    qDebug() << "Please set BUILD_GPXMODEL to ON";
#endif
}

void MainWindow::on_leSrcFile_textChanged(const QString &text)
{
    if(!ui->leDstFile->text().isEmpty())
        return;
    QString szFile;
    szFile = ui->leSrcFile->text();
    QFileInfo f(text);
    szFile = f.path() + QDir::separator() + f.completeBaseName() + "_TC." + f.suffix();
    ui->leDstFile->setText(szFile);
}

void MainWindow::on_pbSrcDir_clicked()
{  
    QString szDir = RabbitCommon::CDir::GetOpenDirectory(this,
                                 tr("Open source directory"));
    if(szDir.isEmpty()) return;
    ui->leSrcDir->setText(szDir);
}

void MainWindow::on_pbDstDir_clicked()
{
    QFileDialog df(this, tr("Open destination directory"));
    QString szDir = RabbitCommon::CDir::GetOpenDirectory(this,
                            tr("Open destination directory"));
    if(szDir.isEmpty()) return;
    ui->leDstDir->setText(szDir);
}

void MainWindow::on_leSrcDir_textChanged(const QString &text)
{
    if(!ui->leDstDir->text().isEmpty())
        return;
    
    ui->leDstDir->setText(text + "_TC");
}

void MainWindow::on_pbConversionDir_clicked()
{
    QString szDir = ui->leDstDir->text();
    QDir dir;
    if(!dir.exists(szDir))
        dir.mkpath(szDir);
    SetStatusInfo(tr("Start transform ......"));
    QCursor cursor = this->cursor();
    this->setCursor(Qt::WaitCursor);
    QDir d(ui->leSrcDir->text());
    int num = 1;
    foreach (QFileInfo f, d.entryInfoList()) {
        if(f.isFile())
        {
#ifdef BUILD_GPXMODEL
            this->statusBar()->showMessage(tr("Be transforming ") + QString::number(num++) + " ......");
            TransformCoordinateFiles(f.filePath().toStdString().c_str(),
                    (ui->leDstDir->text() + QDir::separator() + f.fileName()).toStdString().c_str(),
                    (_COORDINATE)ui->cbSrcCoor->currentIndex(),
                    (_COORDINATE)ui->cbDstCoor->currentIndex());
#else
            qDebug() << "Please set BUILD_GPXMODEL to ON";
#endif
        }
    }
    this->setCursor(cursor);
    SetStatusInfo(tr("Ready"));
    //QMessageBox::information(this, tr("End"), tr("Transform coordinate end"));
}

void MainWindow::on_actionAbout_A_triggered()
{
#ifdef RABBITCOMMON
    CDlgAbout about(this);
    about.m_AppIcon = QImage(":/icon/App");
    about.m_szHomePage = "https://github.com/KangLin/TransformCoordinate";
    if(about.isHidden())
#if defined (Q_OS_ANDROID)
        about.showMaximized();
        about.exec();
#endif
        about.exec();
#endif
}

void MainWindow::on_actionUpdate_U_triggered()
{
#ifdef RABBITCOMMON
    CFrmUpdater* m_pfrmUpdater = new CFrmUpdater();
    m_pfrmUpdater->SetTitle(QImage(":/icon/App"));
    #if defined (Q_OS_ANDROID)
        m_pfrmUpdater->showMaximized();
    #else
        m_pfrmUpdater->show();
    #endif
#endif
}

int MainWindow::InitStatusBar()
{
    this->statusBar()->setVisible(true);
    SetStatusInfo(tr("Ready"));
    m_statusInfo.setSizePolicy(QSizePolicy::Policy::Preferred,
                               QSizePolicy::Policy::Preferred);
    this->statusBar()->addWidget(&m_statusInfo);
    return 0;
}

int MainWindow::SetStatusInfo(QString szText, QColor color)
{
    QPalette pe;
    pe.setColor(QPalette::WindowText, color);
    m_statusInfo.setPalette(pe);
    m_statusInfo.setText(szText);
    return 0;
}
