#include "MainWindow.h"
#include "HuffmanCompressor.h"
#include "HuffmanDecompressor.h"

#include <QApplication>
#include <QDateTime>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QMimeData>
#include <QProgressBar>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTimer>
#include <QVBoxLayout>
#include <QtConcurrent/QtConcurrent>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), statusUpdateTimer(new QTimer(this)) {
    
    setWindowTitle("ByteShrink™ - Professional File Compression");
    setWindowIcon(QIcon(":/icon.png"));
    setAcceptDrops(true);
    resize(1100, 750);
    setMinimumSize(1000, 650);
    
    setupStyles();
    setupUi();
    
    connect(&processWatcher, &QFutureWatcher<void>::finished, this, [this]() {
        processNextFile();
    });
}

void MainWindow::setupStyles() {
    setStyleSheet(R"(
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #f7fafc, stop:1 #edf2f7);
        }
        QFrame {
            background: white;
            border-radius: 12px;
            border: 1px solid #e2e8f0;
        }
        QTableWidget {
            background: white;
            gridline-color: #cbd5e0;
            border: 1px solid #e2e8f0;
            border-radius: 8px;
            color: #1a202c;
        }
        QTableWidget::item {
            padding: 8px;
            border-bottom: 1px solid #e2e8f0;
            color: #2d3748;
            background-color: white;
        }
        QTableWidget::item:selected {
            background-color: #667eea;
            color: white;
        }
        QTableWidget::item:alternate {
            background-color: #f7fafc;
            color: #1a202c;
        }
        QHeaderView::section {
            background: #2d3748;
            color: white;
            padding: 10px;
            border: none;
            border-right: 1px solid #1a202c;
            font-weight: 700;
            font-size: 13px;
        }
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #667eea, stop:1 #764ba2);
            color: white;
            border: none;
            border-radius: 8px;
            font-weight: 700;
            font-size: 13px;
            padding: 10px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #764ba2, stop:1 #667eea);
        }
        QPushButton:pressed {
            background: #5a67d8;
        }
        QPushButton:disabled {
            background: #cbd5e0;
            color: #a0aec0;
        }
        QProgressBar {
            border: none;
            border-radius: 6px;
            background: #edf2f7;
            text-align: center;
            color: #1a202c;
            font-weight: 700;
            font-size: 12px;
        }
        QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #667eea, stop:0.5 #764ba2, stop:1 #f093fb);
            border-radius: 4px;
        }
        QLabel {
            color: #2d3748;
        }
    )");
}

void MainWindow::setupUi() {
    auto* central = new QWidget(this);
    auto* mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    // === HEADER ===
    auto* headerFrame = new QFrame();
    headerFrame->setStyleSheet("background: transparent; border: none;");
    auto* headerLayout = new QHBoxLayout(headerFrame);
    
    auto* logoLabel = new QLabel("🗜️");
    logoLabel->setStyleSheet("font-size: 48px; background: transparent; border: none;");
    
    auto* titleLabel = new QLabel("ByteShrink");
    titleLabel->setStyleSheet(R"(
        font-size: 36px;
        font-weight: 900;
        color: #1a202c;
        background: transparent;
        border: none;
    )");
    
    auto* subtitleLabel = new QLabel("Professional Huffman Compression Engine");
    subtitleLabel->setStyleSheet(R"(
        font-size: 13px;
        color: #718096;
        background: transparent;
        border: none;
        margin-top: 2px;
    )");
    
    auto* titleCol = new QVBoxLayout();
    titleCol->setContentsMargins(0, 0, 0, 0);
    titleCol->setSpacing(0);
    titleCol->addWidget(titleLabel);
    titleCol->addWidget(subtitleLabel);
    
    headerLayout->addWidget(logoLabel);
    headerLayout->addLayout(titleCol);
    headerLayout->addStretch();
    
    // === DROP ZONE ===
    dropZone = new QFrame();
    dropZone->setMinimumHeight(180);
    dropZone->setCursor(Qt::PointingHandCursor);
    dropZone->installEventFilter(this);
    
    auto* dropLayout = new QVBoxLayout(dropZone);
    dropLayout->setAlignment(Qt::AlignCenter);
    dropLayout->setSpacing(12);
    
    auto* dropIconLabel = new QLabel("📂");
    dropIconLabel->setAlignment(Qt::AlignCenter);
    dropIconLabel->setStyleSheet("font-size: 72px; background: transparent; border: none;");
    
    dropLabel = new QLabel("DROP FILES HERE or CLICK TO BROWSE\nSupports all file types • Batch processing");
    dropLabel->setAlignment(Qt::AlignCenter);
    dropLabel->setStyleSheet(R"(
        font-size: 16px;
        font-weight: 600;
        color: #4a5568;
        background: transparent;
        border: none;
        line-height: 1.5;
    )");
    
    dropLayout->addWidget(dropIconLabel);
    dropLayout->addWidget(dropLabel);
    
    // === CONTROL PANEL ===
    auto* controlFrame = new QFrame();
    auto* controlLayout = new QHBoxLayout(controlFrame);
    controlLayout->setSpacing(12);
    
    browseBtn = new QPushButton("📁 Browse Files");
    compressAllBtn = new QPushButton("⚡ Compress All");
    decompressAllBtn = new QPushButton("📦 Decompress All");
    clearQueueBtn = new QPushButton("🗑️ Clear");
    removeBtn = new QPushButton("❌ Remove");
    
    compressAllBtn->setEnabled(false);
    decompressAllBtn->setEnabled(false);
    clearQueueBtn->setEnabled(false);
    removeBtn->setEnabled(false);
    
    controlLayout->addWidget(browseBtn);
    controlLayout->addWidget(compressAllBtn);
    controlLayout->addWidget(decompressAllBtn);
    controlLayout->addWidget(clearQueueBtn);
    controlLayout->addWidget(removeBtn);
    
    // === FILE QUEUE TABLE ===
    fileQueue = new QTableWidget();
    fileQueue->setColumnCount(5);
    fileQueue->setHorizontalHeaderLabels({"File Name", "Size", "Status", "Progress", "Output"});
    fileQueue->horizontalHeader()->setStretchLastSection(false);
    fileQueue->setColumnWidth(0, 300);
    fileQueue->setColumnWidth(1, 100);
    fileQueue->setColumnWidth(2, 120);
    fileQueue->setColumnWidth(3, 150);
    fileQueue->setColumnWidth(4, 250);
    fileQueue->setSelectionBehavior(QAbstractItemView::SelectRows);
    fileQueue->setSelectionMode(QAbstractItemView::SingleSelection);
    fileQueue->setAlternatingRowColors(true);
    fileQueue->setVisible(false);
    
    // === STATS PANEL ===
    auto* statsFrame = new QFrame();
    auto* statsLayout = new QHBoxLayout(statsFrame);
    statsLayout->setSpacing(20);
    
    fileCountLabel = new QLabel("📊 Files: 0");
    fileCountLabel->setStyleSheet(R"(
        font-size: 14px;
        font-weight: 700;
        color: #667eea;
        background: transparent;
        border: none;
    )");
    
    totalSizeLabel = new QLabel("💾 Total: 0 B");
    totalSizeLabel->setStyleSheet(R"(
        font-size: 14px;
        font-weight: 700;
        color: #764ba2;
        background: transparent;
        border: none;
    )");
    
    statsLabel = new QLabel();
    statsLabel->setStyleSheet(R"(
        font-size: 14px;
        color: #718096;
        background: transparent;
        border: none;
    )");
    
    statsLayout->addWidget(fileCountLabel);
    statsLayout->addWidget(totalSizeLabel);
    statsLayout->addWidget(statsLabel, 1);
    statsFrame->setVisible(false);
    
    // === PROGRESS DISPLAY ===
    auto* progressFrame = new QFrame();
    auto* progressLayout = new QVBoxLayout(progressFrame);
    
    auto* progressLabel = new QLabel("Overall Progress");
    progressLabel->setStyleSheet("font-weight: 700; font-size: 14px; background: transparent; border: none;");
    
    overallProgress = new QProgressBar();
    overallProgress->setRange(0, 100);
    overallProgress->setValue(0);
    overallProgress->setFormat("%v / %m files");
    overallProgress->setMinimumHeight(32);
    
    progressLayout->addWidget(progressLabel);
    progressLayout->addWidget(overallProgress);
    progressFrame->setVisible(false);
    
    // === STATUS MESSAGE ===
    statusLabel = new QLabel("Ready to process files");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet(R"(
        font-size: 15px;
        font-weight: 600;
        color: #667eea;
        padding: 16px;
        background: #edf2f7;
        border-radius: 8px;
    )");
    
    // === MAIN ASSEMBLY ===
    mainLayout->addWidget(headerFrame);
    mainLayout->addWidget(dropZone);
    mainLayout->addWidget(controlFrame);
    mainLayout->addWidget(statsFrame);
    mainLayout->addWidget(fileQueue, 1);
    mainLayout->addWidget(progressFrame);
    mainLayout->addWidget(statusLabel);
    
    setCentralWidget(central);
    
    // === CONNECTIONS ===
    connect(browseBtn, &QPushButton::clicked, this, &MainWindow::onBrowseFiles);
    connect(compressAllBtn, &QPushButton::clicked, this, &MainWindow::onCompressAll);
    connect(decompressAllBtn, &QPushButton::clicked, this, &MainWindow::onDecompressAll);
    connect(clearQueueBtn, &QPushButton::clicked, this, &MainWindow::onClearQueue);
    connect(removeBtn, &QPushButton::clicked, this, &MainWindow::onRemoveSelected);
}

void MainWindow::setupTable() {
    fileQueue->setRowCount(files.size());
    
    for (int i = 0; i < files.size(); ++i) {
        QFileInfo info(files[i].filePath);
        
        // Column 0: File Name
        auto* nameItem = new QTableWidgetItem(info.fileName());
        nameItem->setForeground(QColor("#1a202c"));
        fileQueue->setItem(i, 0, nameItem);
        
        // Column 1: Size
        auto* sizeItem = new QTableWidgetItem(formatFileSize(files[i].fileSize));
        sizeItem->setForeground(QColor("#1a202c"));
        sizeItem->setTextAlignment(Qt::AlignCenter);
        fileQueue->setItem(i, 1, sizeItem);
        
        // Column 2: Status
        auto* statusItem = new QTableWidgetItem(files[i].status);
        if (files[i].status == "done") {
            statusItem->setForeground(QColor("#22863a"));
            statusItem->setText("✅ Done");
        } else if (files[i].status == "error") {
            statusItem->setForeground(QColor("#cb2431"));
            statusItem->setText("❌ Error");
        } else if (files[i].status == "compressing") {
            statusItem->setForeground(QColor("#0366d6"));
            statusItem->setText("⚡ Compressing...");
        } else if (files[i].status == "decompressing") {
            statusItem->setForeground(QColor("#6f42c1"));
            statusItem->setText("📦 Decompressing...");
        } else {
            statusItem->setForeground(QColor("#666666"));
            statusItem->setText("⏳ Pending");
        }
        statusItem->setTextAlignment(Qt::AlignCenter);
        fileQueue->setItem(i, 2, statusItem);
        
        // Column 3: Progress Bar
        auto* progressBar = new QProgressBar();
        progressBar->setRange(0, 100);
        progressBar->setValue(files[i].progress);
        progressBar->setFormat("%v%");
        progressBar->setMinimumHeight(25);
        fileQueue->setCellWidget(i, 3, progressBar);
        
        // Column 4: Output
        auto* outputItem = new QTableWidgetItem("");
        outputItem->setForeground(QColor("#0366d6"));
        if (operationMode == 1) {
            QString outputPath = info.dir().filePath(info.completeBaseName() + ".bshk");
            outputItem->setText(QFileInfo(outputPath).fileName());
            outputItem->setToolTip(outputPath);
        } else if (operationMode == 2) {
            QString outputPath = info.dir().filePath(info.completeBaseName() + "_decompressed.txt");
            outputItem->setText(QFileInfo(outputPath).fileName());
            outputItem->setToolTip(outputPath);
        }
        fileQueue->setItem(i, 4, outputItem);
    }
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    if (obj == dropZone && event->type() == QEvent::MouseButtonPress) {
        onBrowseFiles();
        return true;
    }
    if (obj == dropZone && event->type() == QEvent::DragEnter) {
        dragEnterEvent(static_cast<QDragEnterEvent*>(event));
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
        dropZone->setStyleSheet(R"(
            QFrame {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 rgba(102, 126, 234, 0.15), stop:1 rgba(118, 75, 162, 0.15));
                border: 2px dashed #667eea;
                border-radius: 12px;
            }
        )");
    }
}

void MainWindow::dragMoveEvent(QDragMoveEvent* event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent* event) {
    dropZone->setStyleSheet("");
    event->accept();
}

void MainWindow::dropEvent(QDropEvent* event) {
    dropZone->setStyleSheet("");
    if (!event->mimeData()->hasUrls()) return;
    
    QStringList paths;
    for (const auto& url : event->mimeData()->urls()) {
        paths << url.toLocalFile();
    }
    addFilesToQueue(paths);
    event->acceptProposedAction();
}

void MainWindow::onBrowseFiles() {
    QStringList paths = QFileDialog::getOpenFileNames(this, "Select Files", "", "All Files (*)");
    if (!paths.isEmpty()) {
        addFilesToQueue(paths);
    }
}

void MainWindow::addFilesToQueue(const QStringList& paths) {
    for (const auto& path : paths) {
        QFileInfo info(path);
        if (info.isFile()) {
            FileItem item;
            item.filePath = path;
            item.fileSize = info.size();
            item.status = "pending";
            item.progress = 0;
            files.append(item);
        }
    }
    
    if (!files.isEmpty()) {
        fileQueue->setVisible(true);
        dropZone->setVisible(false);
        compressAllBtn->setEnabled(true);
        decompressAllBtn->setEnabled(true);
        clearQueueBtn->setEnabled(true);
        removeBtn->setEnabled(true);
        
        setupTable();
        updateStatsDisplay();
        showStatus("✅ Files added to queue", "#48bb78");
    }
}

void MainWindow::updateStatsDisplay() {
    qint64 totalSize = 0;
    for (const auto& file : files) {
        totalSize += file.fileSize;
    }
    
    fileCountLabel->setText(QString("📊 Files: %1").arg(files.size()));
    totalSizeLabel->setText(QString("💾 Total: %1").arg(formatFileSize(totalSize)));
}

QString MainWindow::formatFileSize(qint64 bytes) const {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int idx = 0;
    double size = bytes;
    while (size >= 1024 && idx < 4) {
        size /= 1024;
        idx++;
    }
    return QString("%1 %2").arg(size, 0, 'f', idx > 0 ? 2 : 0).arg(units[idx]);
}

void MainWindow::showStatus(const QString& msg, const QString& color) {
    statusLabel->setText(msg);
    statusLabel->setStyleSheet(QString(R"(
        font-size: 15px;
        font-weight: 600;
        color: %1;
        padding: 16px;
        background: %2;
        border-radius: 8px;
    )").arg(color).arg(color == "#48bb78" ? "#c6f6d5" : "#fed7d7"));
}

void MainWindow::onCompressAll() {
    if (files.isEmpty()) return;
    operationMode = 1;
    currentProcessing = -1;
    processNextFile();
}

void MainWindow::onDecompressAll() {
    if (files.isEmpty()) return;
    operationMode = 2;
    currentProcessing = -1;
    processNextFile();
}

void MainWindow::onClearQueue() {
    files.clear();
    fileQueue->setRowCount(0);
    fileQueue->setVisible(false);
    dropZone->setVisible(true);
    compressAllBtn->setEnabled(false);
    decompressAllBtn->setEnabled(false);
    clearQueueBtn->setEnabled(false);
    removeBtn->setEnabled(false);
    operationMode = 0;
    currentProcessing = -1;
}

void MainWindow::onRemoveSelected() {
    int row = fileQueue->currentRow();
    if (row >= 0 && row < files.size()) {
        files.removeAt(row);
        setupTable();
        updateStatsDisplay();
        if (files.isEmpty()) {
            onClearQueue();
        }
    }
}

void MainWindow::processNextFile() {
    currentProcessing++;
    if (currentProcessing >= files.size()) {
        showStatus("✅ All files processed successfully!", "#48bb78");
        operationMode = 0;
        overallProgress->setValue(files.size());
        return;
    }
    
    FileItem& current = files[currentProcessing];
    QString input = current.filePath;
    QString output;
    
    if (operationMode == 1) { // Compress
        QFileInfo info(input);
        output = info.dir().filePath(info.completeBaseName() + ".bshk");
        current.status = "compressing";
    } else { // Decompress
        QFileInfo info(input);
        output = info.dir().filePath(info.completeBaseName() + "_decompressed.txt");
        current.status = "decompressing";
    }
    
    setupTable();
    overallProgress->setVisible(true);
    overallProgress->setValue(currentProcessing);
    
    auto future = QtConcurrent::run([this, input, output]() {
        FileItem& current = files[currentProcessing];
        
        try {
            if (operationMode == 1) {
                HuffmanCompressor compressor;
                auto cb = [this](double val) {
                    if (currentProcessing < files.size()) {
                        files[currentProcessing].progress = static_cast<int>(val * 100);
                    }
                };
                compressor.compress(input.toStdString(), output.toStdString(), cb);
                current.status = "done";
            } else {
                HuffmanDecompressor decompressor;
                auto cb = [this](double val) {
                    if (currentProcessing < files.size()) {
                        files[currentProcessing].progress = static_cast<int>(val * 100);
                    }
                };
                decompressor.decompress(input.toStdString(), output.toStdString(), cb);
                current.status = "done";
            }
        } catch (const std::exception& e) {
            current.status = "error";
        }
    });
    
    processWatcher.setFuture(future);
}
