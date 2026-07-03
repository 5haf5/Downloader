#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QMainWindow>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui { class Downloader; }
QT_END_NAMESPACE

class Downloader : public QMainWindow
{
    Q_OBJECT

public:
    explicit Downloader(QWidget* parent = nullptr);
    ~Downloader();

private slots:
    void on_selectFolderButton_clicked();
    void on_downloadButton_clicked();
    void on_btnUpdateYtDlp_clicked();

private:
    Ui::Downloader* ui;

    QString selectedFolder;
    QProcess* process;
    QComboBox* codecBox;
    QProcess* ffmpeg;

    QString ytDlpPath = "C:/Tools/yt-dlp/yt-dlp.exe";
};

#endif
