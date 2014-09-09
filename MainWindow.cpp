#include "MainWindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_total_combinations = 1;
    m_max_coef = 0, m_min_coef = MAX;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open coefficients"), ".",
        tr("All (*.*)"));

    if(fileName!=NULL){
        ui->coef_edit->clear();
        ui->comb_edit->clear();

        //clear
        m_coef.clear();
        m_combination_vector.clear();
        m_total_combinations = 1;
        m_max_coef = 0, m_min_coef = MAX;

        read_file(fileName.toStdString());
        compute_combinations();
        emit on_min_spin_box_valueChanged(ui->min_spin_box->value());
    }
}

void MainWindow::on_actionCredits_triggered()
{
    QMessageBox::about(this, tr("Credits"),
        tr("<h2>Soccer Combinations v1.0</h2>"
        "<p>Copyright &copy; 2014 Jim Stanev \n"
        "<p>Email: jimstanev@gmail.com \n"
        "<p>Computes the combinations of \n"
        "arbitary number of matches and  \n"
        "signs."));
}

void MainWindow::read_file(string path)
{

    fstream file(path.c_str());

    if(!file)
    {
        QMessageBox::about(this, tr("File error"), tr("Can't open the file"));
        return;
    }


    string line;
    int index = 0, comb;
    while(getline(file, line))
    {
        comb = 0;

        stringstream ss(line);

        //qt
        ui->coef_edit->append(QString(line.c_str()));

        m_coef.push_back(vector<double>());
        double value;
        while(ss >> value)
        {
            m_coef[index].push_back(value);
            comb++;
        }
        m_total_combinations *= comb;
        index++;
    }

    file.close();
}

void MainWindow::compute_combinations()
{

    vector_table_i combination_indices;

    //compute combination indexes
    unsigned figures = 1;
    for(unsigned i = 0;i<m_coef.size();i++)
    {
        combination_indices.push_back(vector<int>());

        figures *= m_coef[i].size();
        unsigned step = m_total_combinations/figures;
        unsigned fig_index = 1;
        for(int j = 0;j<m_total_combinations;j++)
        {

            combination_indices[i].push_back(fig_index);
            if((j+1)%step == 0)
            {

                fig_index++;
                if(fig_index > m_coef[i].size())
                {
                    fig_index = 1;
                }
            }
        }
    }
    //print_indexes(indexes);

    //compute combinations
    for(int i = 0;i<m_total_combinations;i++)
    {

        vector<int> temp_choice;
        double temp_coef = 1;
        for(unsigned j = 0;j<combination_indices.size();j++)
        {
            temp_choice.push_back(combination_indices[j].at(i));
            temp_coef *= m_coef[j].at(combination_indices[j].at(i)-1);
        }

        //find max coef
        if(m_max_coef < temp_coef)
        {
            m_max_coef = temp_coef;
        }

        //find min coef
        if(m_min_coef > temp_coef)
        {
            m_min_coef = temp_coef;
        }

        m_combination_vector.push_back(combination(temp_coef, temp_choice));
    }
    //print_combinations(combination_vector, min_coef, max_coef);

    //qt
    ui->min_label->setText(double_to_string(m_min_coef));
    ui->max_label->setText(double_to_string(m_max_coef));
}

void MainWindow::print_combinations(int min, int max)
{

    double min_c = linear_regression(min);
    double max_c = linear_regression(max);
    QString coeff_s = "";

    int comb_counter = 0;
    for(unsigned j = 0;j<m_combination_vector[0].m_choice.size();j++)
    {
        for(int i = 0;i<m_total_combinations;i++)
        {

            if(m_combination_vector[i].m_coef >= min_c &&
                    m_combination_vector[i].m_coef <= max_c)
            {

                coeff_s.append(QVariant(m_combination_vector[i].m_choice[j]).toString() + "\t");
            }

        }
        coeff_s.append("\n");
    }

    for(int i = 0;i<m_total_combinations;i++)
    {

        if(m_combination_vector[i].m_coef >= min_c &&
                m_combination_vector[i].m_coef <= max_c)
        {

            coeff_s.append(double_to_string(m_combination_vector[i].m_coef) + "\t");

            comb_counter++;
        }
    }
    coeff_s.append("\n");

    //qt
    ui->comb_edit->setText(coeff_s);
    ui->comb_label->setText(QVariant(comb_counter).toString());
}

void MainWindow::on_min_spin_box_valueChanged(int min)
{
    if(min >= ui->max_spin_box->value()){
        QMessageBox::about(this, tr("Min error"), tr("Min > Max"));
        return;
    }

    print_combinations(min, ui->max_spin_box->value());
}

void MainWindow::on_max_spin_box_valueChanged(int max)
{
    if(max <= ui->min_spin_box->value()){
        QMessageBox::about(this, tr("Max error"), tr("Min < Max"));
        return;
    }

    print_combinations(ui->min_spin_box->value(), max);
}

double MainWindow::linear_regression(int x)
{
    return ((m_max_coef-m_min_coef)/100)*x + m_min_coef;
}

QString MainWindow::double_to_string(double value)
{
    QString variant = QVariant(value).toString();
    int decimal_point = variant.indexOf('.');

    return variant.toStdString().substr(0, decimal_point + 2).c_str();
}
