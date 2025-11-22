#ifndef CONFIG_H
#define CONFIG_H
#include <iostream>
#include <cstring>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

class Config {
public:
    Config();
    bool loadFile(const std::string filepath);
    bool saveFile(const std::string filepath);

    //try to resolve automatic
    template<typename T>
    static T getConfig(Config &cfg, const char *sector, const char *key)
    {
        const char *v = al_get_config_value(cfg.cfg, sector, key);


        return  static_cast<T>(std::atoi(v));

    }

    static bool  getConfigBool(Config& cfg, const char *sector, const char *key);
    static int   getConfigInt(Config& cfg, const char *sector, const char *key);
    static float getConfigFloat(Config& cfg, const char *sector, const char *key);

    ALLEGRO_CONFIG* createDefaultConfig();
    void Unload();

private:
    ALLEGRO_CONFIG *cfg;
};

#endif // CONFIG_H
