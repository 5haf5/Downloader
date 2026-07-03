#include "stdafx.h"
#include "Downloader.h"
#include "ui_Downloader.h"

#include <QFileDi alog>
#include <QDebug>

Downloader::Downloader(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::Downloader)
{
    ui->setupUi(this);
    process = new QProcess(this);

    // Logování výstupu yt-dlp
    connect(process, &QProcess::readyReadStandardOutput, this, [this]() {
        ui->log->append(process->readAllStandardOutput());
        });

    connect(process, &QProcess::readyReadStandardError, this, [this]() {
        ui->log->append(process->readAllStandardError());
        });

    // Tlačítka
    connect(ui->downloadButton, &QPushButton::clicked,
        this, &Downloader::on_downloadButton_clicked);

    connect(ui->selectFolderButton, &QPushButton::clicked,
        this, &Downloader::on_selectFolderButton_clicked);

    connect(ui->btnUpdateYtDlp, &QPushButton::clicked,
        this, &Downloader::on_btnUpdateYtDlp_clicked);

    // Výběr kvality
    ui->qualityBox->addItem("Hudba (MP3)");
    ui->qualityBox->addItem("360p");
    ui->qualityBox->addItem("480p");
    ui->qualityBox->addItem("720p");
    ui->qualityBox->addItem("1080p");
    ui->qualityBox->addItem("1440p");
    ui->qualityBox->addItem("2160p");
    ui->qualityBox->addItem("4320p");
}

Downloader::~Downloader()
{
    delete ui;
}

void Downloader::on_selectFolderButton_clicked()
{
    QString folder = QFileDialog::getExistingDirectory(this, "Vyber složku pro stažení");

    if (folder.isEmpty()) {
        ui->log->append("❌ Nebyla vybrána žádná složka.");
        return;
    }

    selectedFolder = folder;
    ui->folderEdit->setText(folder);
    ui->log->append("📁 Vybraná složka: " + folder);
}

void Downloader::on_downloadButton_clicked()
{
    QString url = ui->urlEdit->text();

    if (url.isEmpty()) {
        ui->log->append("❌ URL je prázdná!");
        return;
    }

    if (selectedFolder.isEmpty()) {
        ui->log->append("❌ Není vybraná cílová složka!");
        return;
    }

    // Výběr kvality
    QString quality = ui->qualityBox->currentText();
    QString format;
    QStringList args;

    // 🎵 AUDIO ONLY režim
    if (quality.contains("Hudba")) {
        args << "-f" << "bestaudio"
            << "--extract-audio"
            << "--audio-format" << "mp3"
            << "--audio-quality" << "0";
    }
    else {
        // 🎞️ VIDEO režim
        if (quality == "360p")      format = "bestvideo[height<=360]+bestaudio/best";
        else if (quality == "480p") format = "bestvideo[height<=480]+bestaudio/best";
        else if (quality == "720p") format = "bestvideo[height<=720]+bestaudio/best";
        else if (quality == "1080p") format = "bestvideo[height<=1080]+bestaudio/best";
        else if (quality == "1440p") format = "bestvideo[height<=1440]+bestaudio/best";
        else if (quality == "2160p") format = "bestvideo[height<=2160]+bestaudio/best";
        else if (quality == "4320p") format = "bestvideo[height<=4320]+bestaudio/best";
        else format = "best";

        args << "-f" << format;
    }

    QString output = selectedFolder + "/%(title)s [%(id)s].%(ext)s";
    args << "-o" << output;
    args << url;

    ui->log->append("▶️ Spouštím yt-dlp...");
    ui->log->append("🎞️ Kvalita: " + quality);
    ui->log->append("🧩 Cesta k yt-dlp: " + ytDlpPath);
    ui->log->append("🎯 Cílová složka: " + selectedFolder);

    process->start(ytDlpPath, args);

    if (!process->waitForStarted()) {
        ui->log->append("❌ Nepodařilo se spustit yt-dlp.exe!");
    }
}

void Downloader::on_btnUpdateYtDlp_clicked()
{
    ui->log->append("🔄 Kontroluji aktualizaci yt-dlp...");

    QStringList args;
    args << "-U";  // Update command

    process->start(ytDlpPath, args);

    if (!process->waitForStarted()) {
        ui->log->append("❌ Nepodařilo se spustit yt-dlp.exe!");
        return;
    }

    ui->log->append("⬇️ Probíhá aktualizace yt-dlp...");
}
