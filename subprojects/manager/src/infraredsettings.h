#ifndef INFRAREDSETTINGS_H
#define INFRAREDSETTINGS_H

#include <QWidget>

namespace Ui {
    class InfraredSettings;
}

class InfraredSettings : public QWidget
{
    Q_OBJECT

public:
    explicit InfraredSettings(QWidget *parent = 0);
    ~InfraredSettings();

private:
    Ui::InfraredSettings *ui;
};

#endif // INFRAREDSETTINGS_H
