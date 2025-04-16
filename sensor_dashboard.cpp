#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include <thread>
#include <cstdlib>

class Dashboard : public Gtk::Window {
public:
    Dashboard() : label("Ambient Light: 0.00 lux") {
        set_title("Sensor Dashboard");
        set_default_size(400, 200);
        add(label);
        label.show();

        // Start sensor update thread
        std::thread([this]() {
            while (true) {
                // Read sensor data (replace with actual command)
                FILE* pipe = popen("./i2c_veml6030", "r");
                char buffer[128];
                fgets(buffer, sizeof(buffer), pipe);
                pclose(pipe);
                Glib::signal_idle().connect_once([this, buffer]() {
                    label.set_text(buffer);
                });
                sleep(1);
            }
        }).detach();
    }

private:
    Gtk::Label label;
};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.sensor.dashboard");
    Dashboard dashboard;
    return app->run(dashboard);
}
