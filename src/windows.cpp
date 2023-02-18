#include <jack/jack.h>

#include "include/windows.h"
#include "ui_windows.h"

typedef jack_default_audio_sample_t sample_t;

int process(jack_nframes_t nframes, void* data) {
    //printf("Process");
    Widget* widget = static_cast<Widget*>(data);
    sample_t* in_l = static_cast<sample_t*>(jack_port_get_buffer(widget->in_l, nframes));
    sample_t* in_r = static_cast<sample_t*>(jack_port_get_buffer(widget->in_r, nframes));    
    sample_t* out_l = static_cast<sample_t*>(jack_port_get_buffer(widget->out_l, nframes));
    sample_t* out_r = static_cast<sample_t*>(jack_port_get_buffer(widget->out_r, nframes));

    for (unsigned int i = 0; i < nframes; ++i) {
        out_l[i] = widget->gain * in_l[i];
        out_r[i] = widget->gain * in_r[i];
    }

    return 0;
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // init jack garbage
    jack_client = jack_client_open("eg-amp", JackNullOption, nullptr);

    if (jack_client == nullptr)
      throw std::runtime_error("failed to initialize jack!");

    jack_set_process_callback(jack_client, &process, this);

    in_l = jack_port_register(jack_client, "inL", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    if (in_l == nullptr) {
      jack_client_close(jack_client);
      throw std::runtime_error("failed to open jack input port!");
    }

    in_r = jack_port_register(jack_client, "inR", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    if (in_r == nullptr) {
      jack_port_unregister(jack_client, in_l);
      jack_client_close(jack_client);
      throw std::runtime_error("failed to open jack input port!");
    }

    out_l = jack_port_register(jack_client, "outL", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    if (out_l == nullptr) {
      jack_client_close(jack_client);
      throw std::runtime_error("failed to open jack output port!");
    }

    out_r = jack_port_register(jack_client, "outR", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    if (out_r == nullptr) {
      jack_port_unregister(jack_client, out_l);
      jack_client_close(jack_client);
      throw std::runtime_error("failed to open jack output port!");
    }

  sample_rate = jack_get_sample_rate(jack_client);
  connect(ui->dial, &QDial::valueChanged, this, &Widget::updateVol);

  // if all went well, activate it!
  if (jack_activate(jack_client)) {
    jack_port_unregister(jack_client, in_l);
    jack_port_unregister(jack_client, in_r);    
    jack_port_unregister(jack_client, out_l);
    jack_port_unregister(jack_client, out_r);
    jack_client_close(jack_client);
    throw std::runtime_error("could not activate jack client!");
  }    

}

Widget::~Widget()
{
  jack_deactivate(jack_client);
  jack_port_unregister(jack_client, in_l);
  jack_port_unregister(jack_client, in_r);  
  jack_port_unregister(jack_client, out_l);
  jack_port_unregister(jack_client, out_r);
  jack_client_close(jack_client);  
    delete ui;
}

