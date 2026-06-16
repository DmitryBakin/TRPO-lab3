#include "MainWindow.h"

#include "IChartAdapter.h"
#include "IDataLoader.h"
#include "IStyleStrategy.h"
#include "LineChartAdapter.h"
#include "BarChartAdapter.h"
#include "SQLiteLoaderAdapter.h"
#include "JsonLoaderAdapter.h"
#include "ColorStyle.h"
#include "GrayscaleStyle.h"


IOCContainer gContainer;
int IOCContainer::s_nextTypeId = 115094801;

// Простая модель для таблицы данных графика
class DataTableModel : public QAbstractTableModel
{
public:
    DataTableModel(const QVector<QPointF>& data, QObject* parent = nullptr)
        : QAbstractTableModel(parent), m_data(data)
    {
    }

    int rowCount(const QModelIndex& = QModelIndex()) const override
    {
        return m_data.size();
    }

    int columnCount(const QModelIndex& = QModelIndex()) const override
    {
        return 3;
    }

    QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const override
    {
        if (!idx.isValid() || role != Qt::DisplayRole)
            return {};

        const QPointF& p = m_data[idx.row()];
        if (idx.column() == 0)
            return idx.row() + 1;
        if (idx.column() == 1) {
            QDateTime dt;
            dt.setMSecsSinceEpoch(qint64(p.x()));
            return dt.toString("dd.MM.yyyy");
        }
        if (idx.column() == 2)
            return QString::number(p.y(), 'f', 2);
        return {};
    }

    QVariant headerData(int section, Qt::Orientation orient, int role) const override
    {
        if (role != Qt::DisplayRole)
            return {};
        if (orient == Qt::Horizontal) {
            if (section == 0) return "#";
            if (section == 1) return "Дата (X)";
            if (section == 2) return "Значение (Y)";
        }
        return {};
    }

private:
    const QVector<QPointF>& m_data;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_model = new ChartModel(this);
    registerDependencies();
    setupUI();

    connect(m_model, &ChartModel::dataChanged, this, &MainWindow::onModelDataChanged);
    connect(m_model, &ChartModel::dataLoaded, this, &MainWindow::onModelDataLoaded);
    connect(m_model, &ChartModel::errorOccurred, this, &MainWindow::onModelError);

    setWindowTitle("Печать графиков");
    resize(1200, 800);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::registerDependencies()
{
    gContainer.RegisterInstance<IChartAdapter>(std::make_shared<LineChartAdapter>());
    gContainer.RegisterInstance<IStyleStrategy>(std::make_shared<ColorStyle>());
}

void MainWindow::setupUI()
{
    QWidget* central = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(central);

    QWidget* leftPanel = new QWidget(this);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->addWidget(new QLabel("Папки", this));

    m_dirModel = new QFileSystemModel(this);
    m_dirModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    m_dirModel->setRootPath(QDir::homePath());

    m_treeView = new QTreeView(this);
    m_treeView->setModel(m_dirModel);
    m_treeView->setRootIndex(m_dirModel->index(QDir::homePath()));
    m_treeView->setHeaderHidden(true);
    leftLayout->addWidget(m_treeView);
    leftPanel->setMinimumWidth(250);

    QWidget* rightPanel = new QWidget(this);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);

    QWidget* fileArea = new QWidget(this);
    QVBoxLayout* fileLayout = new QVBoxLayout(fileArea);
    fileLayout->addWidget(new QLabel("Файлы в папке", this));

    m_fileModel = new QFileSystemModel(this);
    m_fileModel->setFilter(QDir::Files | QDir::NoDotAndDotDot);
    m_fileModel->setRootPath(QDir::homePath());

    m_fileTableView = new QTableView(this);
    m_fileTableView->setModel(m_fileModel);
    m_fileTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_fileTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_fileTableView->setColumnWidth(0, 200);
    m_fileTableView->setColumnWidth(1, 80);
    m_fileTableView->setColumnWidth(2, 100);
    m_fileTableView->hideColumn(3);
    fileLayout->addWidget(m_fileTableView);

    QHBoxLayout* controlLayout = new QHBoxLayout();
    controlLayout->addWidget(new QLabel("Тип графика:", this));
    m_graphTypeBox = new QComboBox(this);
    m_graphTypeBox->addItem("Линейный", "line");
    m_graphTypeBox->addItem("Столбчатый", "bar");
    m_grayscaleBox = new QCheckBox("Черно-белый график", this);
    m_printBtn = new QPushButton("Печать графика", this);
    controlLayout->addWidget(m_graphTypeBox);
    controlLayout->addWidget(m_grayscaleBox);
    controlLayout->addWidget(m_printBtn);
    controlLayout->addStretch();

    m_chartWidget = new ChartWidget(this);
    m_chartWidget->setMinimumHeight(350);

    m_dataTableView = new QTableView(this);
    m_dataTableView->setAlternatingRowColors(true);
    m_dataTableView->horizontalHeader()->setStretchLastSection(true);

    rightLayout->addWidget(fileArea);
    rightLayout->addLayout(controlLayout);
    rightLayout->addWidget(m_chartWidget, 1);
    rightLayout->addWidget(m_dataTableView, 0);
    rightLayout->setStretchFactor(m_chartWidget, 2);
    rightLayout->setStretchFactor(m_dataTableView, 1);

    // Разделитель
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(leftPanel);
    splitter->addWidget(rightPanel);
    splitter->setSizes({300, 900});
    mainLayout->addWidget(splitter);
    setCentralWidget(central);

    statusBar()->showMessage("Выберите папку, затем файл БД (.db/.sqlite/.json)");

    connect(m_treeView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MainWindow::onFolderSelected);
    connect(m_fileTableView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MainWindow::onFileSelected);
    connect(m_graphTypeBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onGraphTypeChanged);
    connect(m_grayscaleBox, &QCheckBox::stateChanged,
            this, &MainWindow::onStyleChanged);
    connect(m_printBtn, &QPushButton::clicked,
            this, &MainWindow::onPrintPdf);
}

void MainWindow::onFolderSelected(const QModelIndex &index)
{
    if (!index.isValid())
        return;
    QString path = m_dirModel->filePath(index);
    m_fileModel->setRootPath(path);
    m_fileTableView->setRootIndex(m_fileModel->index(path));
}

void MainWindow::onFileSelected(const QModelIndex &index)
{
    if (!index.isValid())
        return;
    QString path = m_fileModel->filePath(index);
    QFileInfo info(path);
    QString tableName = info.baseName();
    if (!info.isFile())
        return;
    if (!(path.endsWith(".db") || path.endsWith(".sqlite") || path.endsWith(".json"))) {
        statusBar()->showMessage("Поддерживаются только .db, .sqlite, .json");
        return;
    }
    m_currentFilePath = path;
    std::unique_ptr<IDataLoader> loader;
    if (path.endsWith(".db") || path.endsWith(".sqlite"))
        loader = std::make_unique<SQLiteLoaderAdapter>();
    else
        loader = std::make_unique<JsonLoaderAdapter>();
    m_model->loadData(path, tableName, loader.get());
}

void MainWindow::onGraphTypeChanged(int)
{
    QString type = m_graphTypeBox->currentData().toString();
    if (type == "line")
        gContainer.RegisterInstance<IChartAdapter>(std::make_shared<LineChartAdapter>());
    else
        gContainer.RegisterInstance<IChartAdapter>(std::make_shared<BarChartAdapter>());

    if (!m_model->isEmpty())
        updateChartView();
}

void MainWindow::onStyleChanged(int state)
{
    if (state == Qt::Checked)
        gContainer.RegisterInstance<IStyleStrategy>(std::make_shared<GrayscaleStyle>());
    else
        gContainer.RegisterInstance<IStyleStrategy>(std::make_shared<ColorStyle>());

    if (!m_model->isEmpty())
        updateChartView();
}

void MainWindow::onPrintPdf()
{
    if (m_model->isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Нет данных для печати");
        return;
    }
    QString pdfPath = QFileDialog::getSaveFileName(this, "Сохранить PDF",
                                                   "graph.pdf", "*.pdf");
    if (pdfPath.isEmpty())
        return;

    auto adapter = gContainer.GetObject<IChartAdapter>();
    QWidget* printWidget = adapter->createChartWidget(m_model->data());
    auto style = gContainer.GetObject<IStyleStrategy>();
    style->apply(printWidget);
    printWidget->resize(800, 600);
    printWidget->show();

    QPdfWriter writer(pdfPath);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(300);
    QPainter painter(&writer);
    printWidget->render(&painter);
    painter.end();

    delete printWidget;
    statusBar()->showMessage("PDF сохранён: " + pdfPath);
    QMessageBox::information(this, "Успех", "График сохранён в PDF");
}

void MainWindow::onModelDataChanged()
{
    updateChartView();
    updateDataTableView();
}

void MainWindow::onModelDataLoaded(int count)
{
    statusBar()->showMessage(QString("Загружено %1 точек из %2")
                                 .arg(count)
                                 .arg(QFileInfo(m_currentFilePath).fileName()));
}

void MainWindow::onModelError(const QString& error)
{
    QMessageBox::warning(this, "Ошибка загрузки", error);
    statusBar()->showMessage("Ошибка загрузки данных");
}

void MainWindow::updateChartView()
{
    if (!m_chartWidget) return;

    if (m_model->isEmpty()) {
        m_chartWidget->setData({});
        return;
    }

    QString type = m_graphTypeBox->currentData().toString();
    m_chartWidget->setChartType(type);

    m_chartWidget->setData(m_model->data());

    auto style = gContainer.GetObject<IStyleStrategy>();
    style->apply(m_chartWidget);
}

void MainWindow::updateDataTableView()
{
    DataTableModel* tableModel = new DataTableModel(m_model->data(), this);
    m_dataTableView->setModel(tableModel);
    m_dataTableView->resizeColumnsToContents();
}
