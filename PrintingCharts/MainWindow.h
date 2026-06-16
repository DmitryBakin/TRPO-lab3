#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>

class ChartModel;
class ChartWidget;
class QTreeView;
class QTableView;
class QComboBox;
class QCheckBox;
class QPushButton;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onFolderSelected(const QModelIndex &index);
    void onFileSelected(const QModelIndex &index);
    void onGraphTypeChanged(int);
    void onStyleChanged(int state);
    void onPrintPdf();
    void onModelDataChanged();
    void onModelDataLoaded(int count);
    void onModelError(const QString& error);

private:
    void setupUI();
    void registerDependencies();
    void updateChartView();
    void updateDataTableView();

    Ui::MainWindow *ui;

    ChartModel* m_model = nullptr;
    ChartWidget* m_chartWidget = nullptr;
    QTableView* m_dataTableView = nullptr;

    QFileSystemModel* m_dirModel = nullptr;
    QTreeView* m_treeView = nullptr;
    QFileSystemModel* m_fileModel = nullptr;
    QTableView* m_fileTableView = nullptr;

    QComboBox* m_graphTypeBox = nullptr;
    QCheckBox* m_grayscaleBox = nullptr;
    QPushButton* m_printBtn = nullptr;

    QString m_currentFilePath;
};

#endif // MAINWINDOW_H
