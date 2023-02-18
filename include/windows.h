#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <jack/jack.h>
#include <mutex>
#include <atomic>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

    public:
        jack_port_t * in_l;
        jack_port_t * in_r;
        jack_port_t * out_l;
        jack_port_t * out_r;

        std::atomic<float> gain;
        std::mutex enabled_mutex;
        
        Widget(QWidget *parent = nullptr);
        ~Widget();

    private:
        jack_client_t* jack_client;
        int sample_rate;

        Ui::Widget *ui;

    private slots:
        void updateVol(int val) {gain = val / 100.f;}
};
#endif // WIDGET_H
