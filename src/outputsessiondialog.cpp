#include "outputsessiondialog.h"
#include "ui_outputsessiondialog.h"
#include "db/db-exception.h"
#include <QMessageBox>

OutputSessionDialog::OutputSessionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OutputSessionDialog),
    m_selectedId{0}
{
    ui->setupUi(this);
    setFixedSize(size());
    setWindowTitle("Sessions availble to play");
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QObject::connect(ui->tableView, SIGNAL(clicked(QModelIndex)),
                     this, SLOT(slotRowSelected(QModelIndex)));
}

void
OutputSessionDialog::useReader(const MM2Capture::DBReader &reader)
{
    try {
        QSqlQuery query = reader.getAvailableSessions();
        m_currentModel.setQuery(query);
        m_currentModel.setHeaderData(0, Qt::Horizontal, "Start time");
        m_currentModel.setHeaderData(1, Qt::Horizontal, "End Time");
        m_currentModel.setHeaderData(2, Qt::Horizontal, "Session name");
        ui->tableView->setModel(&m_currentModel);
    } catch (const DBException& exc) {
        QMessageBox::critical(this, "SQL Error",
                              exc.what(), QMessageBox::Ok);
        reject();
    }
}

void
OutputSessionDialog::slotRowSelected(const QModelIndex &ind) {
    m_selectedId = ind.row();
}

OutputSessionDialog::~OutputSessionDialog()
{
    delete ui;
}
