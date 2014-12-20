#include "Settings.h"
#include <QFile>
#include <QSettings>

Settings settings;

/**
 * Loads the application settings, or, if no saved settings are available,
 * loads default values for the settings. You can change the defaults here.
 */
void Settings::loadSettingsOrDefaults() {
    // Set the default values below
    QSettings s;

    // Example for how to load:
    // fillMode = s.value("fillMode", FILL_SHADED).toInt();
    // lightingEnabled = s.value("lightingEnabled", false).toBool();
    // shadingMode = s.value("shadingMode", SHADING_SMOOTH).toInt();
}

void Settings::saveSettings() {
    QSettings s;

    // Example for how to save
    // s.setValue("fillMode", fillMode);
    // s.setValue("lightingEnabled", lightingEnabled);
    // s.setValue("shadingMode", shadingMode);
}
