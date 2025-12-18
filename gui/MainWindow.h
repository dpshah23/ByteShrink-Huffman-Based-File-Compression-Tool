#pragma once

#include <QFutureWatcher>
#include <QMainWindow>
#include <QTableWidget>
#include <QProgressBar>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QTimer>
#include <QList>

struct FileItem {
    QString filePath;
    qint64 fileSize;
    QString status;
    int progress;
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
    void onBrowseFiles();
    void onCompressAll();
    void onDecompressAll();
    void onClearQueue();
    void onRemoveSelected();
    void processNextFile();

private:
    // UI Elements
    QTableWidget* fileQueue;
    QProgressBar* overallProgress;
    QLabel* statsLabel;
    QLabel* statusLabel;
    QPushButton* browseBtn;
    QPushButton* compressAllBtn;
    QPushButton* decompressAllBtn;
    QPushButton* clearQueueBtn;
    QPushButton* removeBtn;
    QFrame* dropZone;
    QLabel* dropLabel;
    QLabel* fileCountLabel;
    QLabel* totalSizeLabel;

    // Data
    QList<FileItem> files;
    int currentProcessing = -1;
    bool isProcessing = false;
    int operationMode = 0; // 0 = idle, 1 = compress, 2 = decompress

    // Watchers
    QFutureWatcher<void> processWatcher;
    QTimer* statusUpdateTimer;

    // Methods
    void setupUi();
    void setupStyles();
    void setupTable();
    void addFilesToQueue(const QStringList& paths);
    void updateStatsDisplay();
    QString formatFileSize(qint64 bytes) const;
    void showStatus(const QString& msg, const QString& color);
};
