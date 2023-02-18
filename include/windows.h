/* window.h
 *
 * Copyright (C) 2023 https://github.com/lherg
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
