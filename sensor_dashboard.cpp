#include <gtkmm.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

class SensorDashboard : public Gtk::Window {
public:
    SensorDashboard() {
        set_title("Ambient Light Dashboard");
        set_default_size(300, 200);
        label.set_text("Lux: 0.00");
        add(label);
        show_all();

        // Open FIFO
        fifo_fd = open("/tmp/sensor_fifo", O_RDONLY | O_NONBLOCK);
        if (fifo_fd < 0) {
            std::cerr << "Failed to open FIFO" << std::endl;
            return;
        }

        // Set up IOChannel
        channel = Glib::IOChannel::create_from_fd(fifo_fd);
        channel->set_encoding("");
        channel->set_buffered(false);
        Glib::signal_io().connect(sigc::mem_fun(*this, &SensorDashboard::on_data_available), 
                                 channel, Glib::IO_IN);
    }

    ~SensorDashboard() {
        if (fifo_fd >= 0) ::close(fifo_fd);
    }

private:
    Gtk::Label label;
    int fifo_fd = -1;
    Glib::RefPtr<Glib::IOChannel> channel;

    bool on_data_available(Glib::IOCondition cond) {
        if (cond & Glib::IO_IN) {
            Glib::ustring line;
            Glib::IOStatus status = channel->read_line(line);
            if (status == Glib::IO_STATUS_NORMAL && !line.empty()) {
                label.set_text(line);
            }
        }
        return true; // Keep watching
    }
};

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.example.sensordashboard");
    SensorDashboard window;
    return app->run(window);
}
