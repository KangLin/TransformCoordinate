// 作者：康 林 <kl222@126.com>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "TransformCoordinate.h"
#include <QLoggingCategory>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QCursor>
#include <QLocale>
#include <QStatusBar>
#include <QStandardPaths>

#ifdef RABBITCOMMON
    #include "DlgAbout.h"
    #include "FrmUpdater.h"
    #include "RabbitCommonDir.h"
    #include "RabbitCommonTools.h"
    #include "FrmStyle.h"
#endif

static Q_LOGGING_CATEGORY(log, "main")
    
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->actionAbout_A->setIcon(windowIcon());

    InitStatusBar();
    
    CFrmUpdater updater;
    ui->actionUpdate_U->setIcon(updater.windowIcon());
    RabbitCommon::CTools::InsertStyleMenu(ui->menuTools, ui->actionExit);
    ui->menuTools->insertMenu(ui->actionExit,
                              RabbitCommon::CTools::GetLogMenu(this));
    ui->menuTools->insertSeparator(ui->actionExit);
    
    QStringList lstCoor;
    lstCoor << "WGS84" << "GCJ02" << "BD09LL" << "BD09MC";
    ui->cbSrcCoor->addItems(lstCoor);
    ui->cbSrcCoor->setCurrentIndex(WGS84);
    ui->cbDstCoor->addItems(lstCoor);
    ui->cbDstCoor->setCurrentIndex(GCJ02);

    RabbitCommon::CTools::RestoreWidget(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    RabbitCommon::CTools::SaveWidget(this);
    QMainWindow::closeEvent(event);
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
        qCritical(log) << tr("Convert fail:") << nRet;
        SetStatusInfo(tr("Convert fail"), Qt::red);
        return;
    }

    SetStatusInfo(tr("Ready"));
    ui->leDstLong->setText(QString::number(x1));
    ui->leDstLat->setText(QString::number(y1));
}

void MainWindow::on_pbBrowsSrcFile_clicked()
{
    QString szExt = tr("GPX file(*.gpx);;NMea file(*.nmea);;ACT file(*.act);;txt(*.txt);;All files(*.*)");
    QString szFile = QFileDialog::getOpenFileName(this, 
                                        tr("Open source file"),
                                        QStandardPaths::writableLocation(
                                            QStandardPaths::DocumentsLocation),
                                                         szExt);
    if(szFile.isEmpty()) return;
    ui->leSrcFile->setText(szFile);
}

void MainWindow::on_pbBrowsDstFile_clicked()
{
    QString szExt = tr("GPX file(*.gpx);;");
#ifdef BUILD_LIBKML
	szExt += tr("KML file(*.kml)");
#endif
    QString szFile = QFileDialog::getOpenFileName(this, 
                                  tr("Open destination file"),
                                  QStandardPaths::writableLocation(
                                       QStandardPaths::DocumentsLocation),
                                  szExt);
    if(szFile.isEmpty()) return;
    ui->leDstFile->setText(szFile);
}

void MainWindow::on_pbConversionFile_clicked()
{
#ifdef WITH_GPXMODEL
    SetStatusInfo(tr("Start transform ......"));
    QCursor cursor = this->cursor();
    this->setCursor(Qt::WaitCursor);
    int nRet = TransformCoordinateFiles(ui->leSrcFile->text().toStdString().c_str(),
                    ui->leDstFile->text().toStdString().c_str(),
                    (_COORDINATE)ui->cbSrcCoor->currentIndex(),
                    (_COORDINATE)ui->cbDstCoor->currentIndex());
    this->setCursor(cursor);
    if(nRet) {
        qDebug(log) << "Transform Coordinate files fail:" << nRet;
        SetStatusInfo(tr("Convert files fail"), Qt::red);
    } else {
        //QMessageBox::information(this, tr("End"), tr("Transform coordinate end"));
        SetStatusInfo(tr("Ready"));
    }
#else
    qDebug(log) << "Please set WITH_GPXMODEL to ON";
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
    QString szDir = QFileDialog::getExistingDirectory(this,
                                 tr("Open source directory"),
                                 QStandardPaths::writableLocation(
                                 QStandardPaths::DocumentsLocation));
    if(szDir.isEmpty()) return;
    ui->leSrcDir->setText(szDir);
}

void MainWindow::on_pbDstDir_clicked()
{
    QString szDir = QFileDialog::getExistingDirectory(this,
                            tr("Open destination directory"),
                            QStandardPaths::writableLocation(
                               QStandardPaths::DocumentsLocation));
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
#ifdef WITH_GPXMODEL
            SetStatusInfo(tr("Be transforming ") + QString::number(num++) + " ......");
            TransformCoordinateFiles(f.filePath().toStdString().c_str(),
                    (ui->leDstDir->text() + QDir::separator() + f.fileName()).toStdString().c_str(),
                    (_COORDINATE)ui->cbSrcCoor->currentIndex(),
                    (_COORDINATE)ui->cbDstCoor->currentIndex());
#else
            qDebug(log) << "Please set WITH_GPXMODEL to ON";
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
    QIcon icon = windowIcon();
    if(!icon.isNull()) {
        auto sizeList = icon.availableSizes();
        if(sizeList.isEmpty()) return;
        QPixmap p = icon.pixmap(*sizeList.begin());
        about.m_AppIcon = p.toImage();
    }
    about.m_szHomePage = "https://github.com/KangLin/TransformCoordinate";
    about.m_szCopyrightStartTime = "2018";
    about.m_szVersion = TransformCoordinate_VERSION;
    about.m_szVersionRevision = TransformCoordinate_REVISION;
    RC_SHOW_WINDOW(&about);
#endif
}

void MainWindow::on_actionUpdate_U_triggered()
{
#ifdef RABBITCOMMON
    CFrmUpdater* m_pfrmUpdater = new CFrmUpdater();
    m_pfrmUpdater->SetTitle(QImage(":/icon/App"));
    RC_SHOW_WINDOW(m_pfrmUpdater);
#endif
}

int MainWindow::InitStatusBar()
{
    this->statusBar()->setVisible(true);
    SetStatusInfo(tr("Ready"));
    m_statusInfo.setSizePolicy(QSizePolicy::Policy::Expanding,
                               QSizePolicy::Policy::Preferred);
    this->statusBar()->addWidget(&m_statusInfo);
    return 0;
}

int MainWindow::SetStatusInfo(QString szText, QColor color)
{
    if(color.isValid()) {
        QPalette pe;
        pe.setColor(QPalette::WindowText, color);
        m_statusInfo.setPalette(pe);
    }
    m_statusInfo.setText(szText);
    return 0;
}

void MainWindow::on_actionExit_triggered()
{
    close();
}
