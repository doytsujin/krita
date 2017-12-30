/*
 *  Copyright (c) 2008 Boudewijn Rempt <boud@valdyas.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include "kis_paint_action_type_option.h"
#include <klocalizedstring.h>

#include <QWidget>
#include <QRadioButton>

#include "ui_wdgincremental.h"
#include <KisPaintopSettingsIds.h>

class KisPaintActionWidget: public QWidget, public Ui::WdgIncremental
{
public:
    KisPaintActionWidget(QWidget *parent = 0)
        : QWidget(parent) {
        setupUi(this);
    }
};


KisPaintActionTypeOption::KisPaintActionTypeOption()
    : KisPaintOpOption(KisPaintOpOption::COLOR, false)
{
    setObjectName("KisPaintActionTypeOption");

    m_checkable = false;
    m_optionWidget = new KisPaintActionWidget();
    connect(m_optionWidget->radioBuildup, SIGNAL(toggled(bool)), SLOT(emitSettingChanged()));
    connect(m_optionWidget->radioWash, SIGNAL(toggled(bool)), SLOT(emitSettingChanged()));

    m_optionWidget->hide();
    setConfigurationPage(m_optionWidget);
}


KisPaintActionTypeOption::~KisPaintActionTypeOption()
{
}


enumPaintActionType KisPaintActionTypeOption::paintActionType() const
{
    if (m_optionWidget->radioBuildup->isChecked()) {
        return BUILDUP;
    }
    else if (m_optionWidget->radioWash->isChecked()) {
        return WASH;
    }
    else {
        return WASH;
    }
}

void KisPaintActionTypeOption::writeOptionSetting(KisPropertiesConfigurationSP setting) const
{
    setting->setProperty("PaintOpAction", paintActionType());
}

void KisPaintActionTypeOption::readOptionSetting(const KisPropertiesConfigurationSP setting)
{
    const bool isMaskingBrushEnabled = setting->getBool(KisPaintOpUtils::MaskingBrushEnabledTag);

    if (!isMaskingBrushEnabled) {
        enumPaintActionType type = (enumPaintActionType)setting->getInt("PaintOpAction", WASH);
        m_optionWidget->radioBuildup->setChecked(type == BUILDUP);
        m_optionWidget->radioWash->setChecked(type == WASH);
    } else {
        m_optionWidget->radioWash->setChecked(true);
    }

    updateControlsAvailability(!isMaskingBrushEnabled);
}

void KisPaintActionTypeOption::slotForceWashMode(bool value)
{
    if (value) {
        m_optionWidget->radioWash->setChecked(true);
    }

    // TODO: We do not restore the state of the painting
    //       mode before masking was activated! Hope noone
    //       will notice it, since BuildUp mode is not used
    //       too often :)

    updateControlsAvailability(!value);
}

void KisPaintActionTypeOption::updateControlsAvailability(bool value)
{
    m_optionWidget->radioBuildup->setEnabled(value);
    m_optionWidget->radioWash->setEnabled(value);
    m_optionWidget->label->setEnabled(value);

    const QString &toolTipText =
        i18nc("@info:tooltip", "Wash Mode is explicitly set by Masking Brush feature");

    m_optionWidget->radioBuildup->setToolTip(toolTipText);
    m_optionWidget->radioWash->setToolTip(toolTipText);
    m_optionWidget->label->setToolTip(toolTipText);
}

