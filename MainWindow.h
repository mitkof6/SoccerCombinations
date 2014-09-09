#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QMessageBox>
#include <QFileDialog>

#include <vector>
#include <sstream>
#include <fstream>
#include <string>


using namespace std;

#define MAX 9999999999

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_actionOpen_triggered();

    void on_actionCredits_triggered();

    void on_min_spin_box_valueChanged(int min);

    void on_max_spin_box_valueChanged(int max);

private:
    Ui::MainWindow *ui;

    struct combination{
        double m_coef;
        vector<int> m_choice;

        combination(double coef, vector<int> choice){
            m_coef = coef;
            m_choice = choice;
        }
    };

    typedef vector< vector<double> > vector_table_d;
    typedef vector< vector<int> > vector_table_i;
    typedef vector< combination > combinations;

    vector_table_d m_coef;
    combinations m_combination_vector;
    int m_total_combinations;
    double m_max_coef, m_min_coef;

    void read_file(string path);

    void compute_combinations();

    void print_combinations(int min, int max);

    double linear_regression(int x);

    QString double_to_string(double value);
};

#endif // MAINWINDOW_H
