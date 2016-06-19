#ifndef EXPLAIN_H
#define EXPLAIN_H

#include <QDialog>

namespace Ui {
class Explain;
}

class Explain : public QDialog
{
    Q_OBJECT

public:
    explicit Explain(QWidget *parent = 0);
    ~Explain();

private:
    Ui::Explain *ui;
};

#endif // EXPLAIN_H
