/*
 *   This file is part of PKSM
 *   Copyright (C) 2016-2019 Bernardo Giordano, Admiral Fish, piepie62
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
 *       * Requiring preservation of specified reasonable legal notices or
 *         author attributions in that material or in the Appropriate Legal
 *         Notices displayed by works containing it.
 *       * Prohibiting misrepresentation of the origin of that material,
 *         or requiring that modified versions of such material be marked in
 *         reasonable ways as different from the original version.
 */

#include "SaveLoadScreen.hpp"
#include "AccelButton.hpp"
#include "ClickButton.hpp"
#include "ConfigScreen.hpp"
#include "MainMenu.hpp"
#include "TitleLoadScreen.hpp"

static constexpr std::string_view dsIds[9] = {
    "ADA", // Diamond
    "APA", // Pearl
    "CPU", // Platinum
    "IPK", // HeartGold
    "IPG", // SoulSilver
    "IRB", // Black
    "IRA", // White
    "IRE", // Black 2
    "IRD"  // White 2
};

static constexpr std::string_view ctrIds[] = {"0x0055D", "0x0055E", "0x011C4", "0x011C5", "0x01648", "0x0175E", "0x01B50", "0x01B51"};

static constexpr std::string_view ctrNames[] = {"XY", "ORAS", "SUMO", "USUM"};

static constexpr std::string_view dsNames[] = {"Pt", "DP", "HGSS", "BW", "B2W2"};

SaveLoadScreen::SaveLoadScreen()
    : Screen(
          i18n::localize("A_SELECT") + '\n' + i18n::localize("X_SETTINGS") + '\n' + i18n::localize("Y_PRESENT") + '\n' + i18n::localize("START_EXIT"))
{
    buttons.push_back(
        std::make_unique<AccelButton>(24, 96, 175, 16, [this]() { return this->setSelectedSave(0); }, ui_sheet_res_null_idx, "", 0.0f, 0, 10, 10));
    for (int i = 1; i < 5; i++)
    {
        buttons.push_back(std::make_unique<ClickButton>(
            24, 96 + 17 * i, 175, 16, [this, i]() { return this->setSelectedSave(i); }, ui_sheet_res_null_idx, "", 0.0f, 0));
    }
    buttons.push_back(
        std::make_unique<AccelButton>(24, 181, 175, 16, [this]() { return this->setSelectedSave(5); }, ui_sheet_res_null_idx, "", 0.0f, 0, 10, 10));
    buttons.push_back(std::make_unique<Button>(200, 95, 96, 51, [this]() { return this->loadSave(); }, ui_sheet_res_null_idx, "", 0.0f, 0));
    buttons.push_back(std::make_unique<Button>(200, 147, 96, 51, &receiveSaveFromBridge, ui_sheet_res_null_idx, "", 0.0f, 0));

    for (auto i = TitleLoader::sdSaves.begin(); i != TitleLoader::sdSaves.end(); i++)
    {
        std::string key = i->first;
        if (key.size() == 4)
        {
            if (key.substr(0, 3) == dsIds[0])
            {
                for (size_t j = 0; j < i->second.size(); j++)
                {
                    saves[0].push_back({"D: ", i->second[j]});
                }
            }
            else if (key.substr(0, 3) == dsIds[1])
            {
                for (size_t j = 0; j < i->second.size(); j++)
                {
                    saves[0].push_back({"P: ", i->second[j]});
                }
            }
            else if (key.substr(0, 3) == dsIds[2])
            {
                for (size_t j = 0; j < i->second.size(); j++)
                {
                    saves[-1].push_back({"Pt: ", i->second[j]});
                }
            }
            else if (key.substr(0, 3) == dsIds[3])
            {
                for (size_t j = 0; j < i->second.size(); j++)
                {
                    saves[1].push_back({"HG: ", i->second[j]});
                }
            }
            else if (key.substr(0, 3) == dsIds[4])
            {
                for (size_t j = 0; j < i->second.size(); j++)
                {
                    saves[1].push_back({"SS: ", i->second[j]});
                }
            }
            else if (key.substr(0, 3) == dsIds[5])
            {
                for (size_t j = 0; j < i->second.size(); j++)
                {
                    saves[2].push_back({"B: ", i->second[j]});
                }
            }
            else if (key.substr(0, 3) == dsIds[6])
            {
                for (size_t j = 0; j < i->second.size(); j++)
                {
                    saves[2].push_back({"W: ", i->second[j]});
                }
            }
            else if (key.substr(0, 3) == dsIds[7])
            {
                for (size_t j = 0; j < i->second.size(); j++)
                {
                    saves[3].push_back({"B2: ", i->second[j]});
                }
            }
            else if (key.substr(0, 3) == dsIds[8])
            {
                for (size_t j = 0; j < i->second.size(); j++)
                {
                    saves[3].push_back({"W2: ", i->second[j]});
                }
            }
        }
        else
        {
            if (key == ctrIds[0])
            {
                for (size_t j = 0; j < i->second.size(); j++)
                {
                    saves[4].push_back({"X: ", i->second[j]});
                }
            }
            else if (key == ctrIds[1])
            {
                for (size_t j = 0; j < i->second.size(); j++)
                {
                    saves[4].push_back({"Y: ", i->second[j]});
                }
            }
            else if (key == ctrIds[2])
            {
                for (size_t j = 0; j < i->second.size(); j++)
                {
                    saves[5].push_back({"OR: ", i->second[j]});
                }
            }
            else if (key == ctrIds[3])
            {
                for (size_t j = 0; j < i->second.size(); j++)
                {
                    saves[5].push_back({"AS: ", i->second[j]});
                }
            }
            else if (key == ctrIds[4])
            {
                for (size_t j = 0; j < i->second.size(); j++)
                {
                    saves[6].push_back({"S: ", i->second[j]});
                }
            }
            else if (key == ctrIds[5])
            {
                for (size_t j = 0; j < i->second.size(); j++)
                {
                    saves[6].push_back({"M: ", i->second[j]});
                }
            }
            else if (key == ctrIds[6])
            {
                for (size_t j = 0; j < i->second.size(); j++)
                {
                    saves[7].push_back({"US: ", i->second[j]});
                }
            }
            else if (key == ctrIds[7])
            {
                for (size_t j = 0; j < i->second.size(); j++)
                {
                    saves[7].push_back({"UM: ", i->second[j]});
                }
            }
        }
    }
}

void SaveLoadScreen::drawTop(void) const
{
    Gui::drawSolidRect(0, 0, 400.0f, 240.0f, C2D_Color32(15, 22, 89, 255));

    Gui::sprite(ui_sheet_emulated_gameselector_bg_idx, 4, 29);
    Gui::sprite(ui_sheet_gameselector_cart_idx, 35, 93);

    int x = 90;
    int y = 68;

    // draw DS game boxes
    Gui::drawSolidRect(x += 60, y, 48, 48, COLOR_HIGHBLUE);
    Gui::pkm(483, 0, Generation::SEVEN, 2, x - 4, y - 2);
    Gui::text("D", x + 48 - 9, y + 12, FONT_SIZE_11, FONT_SIZE_11, COLOR_WHITE, TextPosX::CENTER, TextPosY::CENTER);
    Gui::pkm(484, 0, Generation::SEVEN, 2, x + 18, y + 20);
    Gui::text("P", x + 9, y + 37, FONT_SIZE_11, FONT_SIZE_11, COLOR_WHITE, TextPosX::CENTER, TextPosY::CENTER);
    Gui::drawSolidRect(x += 60, y, 48, 48, COLOR_HIGHBLUE);
    Gui::pkm(250, 0, Generation::SEVEN, 2, x - 5, y - 2);
    Gui::text("HG", x + 48 - 9, y + 12, FONT_SIZE_11, FONT_SIZE_11, COLOR_WHITE, TextPosX::CENTER, TextPosY::CENTER);
    Gui::pkm(249, 0, Generation::SEVEN, 2, x + 17, y + 20);
    Gui::text("SS", x + 9, y + 37, FONT_SIZE_11, FONT_SIZE_11, COLOR_WHITE, TextPosX::CENTER, TextPosY::CENTER);
    Gui::drawSolidRect(x += 60, y, 48, 48, COLOR_HIGHBLUE);
    Gui::pkm(643, 0, Generation::SEVEN, 2, x - 3, y - 2);
    Gui::text("B", x + 48 - 9, y + 12, FONT_SIZE_11, FONT_SIZE_11, COLOR_WHITE, TextPosX::CENTER, TextPosY::CENTER);
    Gui::pkm(644, 0, Generation::SEVEN, 2, x + 18, y + 20);
    Gui::text("W", x + 9, y + 37, FONT_SIZE_11, FONT_SIZE_11, COLOR_WHITE, TextPosX::CENTER, TextPosY::CENTER);
    Gui::drawSolidRect(x += 60, y, 48, 48, COLOR_HIGHBLUE);
    Gui::pkm(646, 2, Generation::SEVEN, 2, x - 6, y - 2);
    Gui::text("B2", x + 48 - 9, y + 12, FONT_SIZE_11, FONT_SIZE_11, COLOR_WHITE, TextPosX::CENTER, TextPosY::CENTER);
    Gui::pkm(646, 1, Generation::SEVEN, 2, x + 16, y + 20);
    Gui::text("W2", x + 11, y + 37, FONT_SIZE_11, FONT_SIZE_11, COLOR_WHITE, TextPosX::CENTER, TextPosY::CENTER);
    Gui::drawSolidRect(40, 98, 48, 48, COLOR_HIGHBLUE);
    Gui::pkm(487, 1, Generation::SEVEN, 2, 48, 98);
    Gui::text("Pt", 40 + 24, 98 + 30, FONT_SIZE_11, FONT_SIZE_11, COLOR_WHITE, TextPosX::CENTER, TextPosY::TOP);

    x = 90, y = 128;
    // draw 3DS game boxes
    Gui::drawSolidRect(x += 60, y, 48, 48, COLOR_HIGHBLUE);
    Gui::pkm(716, 0, Generation::SEVEN, 2, x - 3, y - 2);
    Gui::text("X", x + 48 - 9, y + 12, FONT_SIZE_11, FONT_SIZE_11, COLOR_WHITE, TextPosX::CENTER, TextPosY::CENTER);
    Gui::pkm(717, 0, Generation::SEVEN, 2, x + 16, y + 20);
    Gui::text("Y", x + 9, y + 37, FONT_SIZE_11, FONT_SIZE_11, COLOR_WHITE, TextPosX::CENTER, TextPosY::CENTER);
    Gui::drawSolidRect(x += 60, y, 48, 48, COLOR_HIGHBLUE);
    Gui::pkm(383, 1, Generation::SEVEN, 2, x - 2, y - 2);
    Gui::text("OR", x + 48 - 7, y + 12, FONT_SIZE_11, FONT_SIZE_11, COLOR_WHITE, TextPosX::CENTER, TextPosY::CENTER);
    Gui::pkm(382, 1, Generation::SEVEN, 2, x + 16, y + 20);
    Gui::text("AS", x + 9, y + 37, FONT_SIZE_11, FONT_SIZE_11, COLOR_WHITE, TextPosX::CENTER, TextPosY::CENTER);
    Gui::drawSolidRect(x += 60, y, 48, 48, COLOR_HIGHBLUE);
    Gui::pkm(791, 0, Generation::SEVEN, 2, x - 4, y - 2);
    Gui::text("S", x + 48 - 9, y + 12, FONT_SIZE_11, FONT_SIZE_11, COLOR_WHITE, TextPosX::CENTER, TextPosY::CENTER);
    Gui::pkm(792, 0, Generation::SEVEN, 2, x + 18, y + 20);
    Gui::text("M", x + 9, y + 37, FONT_SIZE_11, FONT_SIZE_11, COLOR_WHITE, TextPosX::CENTER, TextPosY::CENTER);
    Gui::drawSolidRect(x += 60, y, 48, 48, COLOR_HIGHBLUE);
    Gui::pkm(800, 1, Generation::SEVEN, 2, x - 3, y - 2);
    Gui::text("US", x + 48 - 9, y + 12, FONT_SIZE_11, FONT_SIZE_11, COLOR_WHITE, TextPosX::CENTER, TextPosY::CENTER);
    Gui::pkm(800, 2, Generation::SEVEN, 2, x + 19, y + 20);
    Gui::text("UM", x + 11, y + 37, FONT_SIZE_11, FONT_SIZE_11, COLOR_WHITE, TextPosX::CENTER, TextPosY::CENTER);

    if (saveGroup == -1)
    {
        Gui::drawSelector(39, 97);
    }
    else if (saveGroup < 4)
    {
        Gui::drawSelector(149 + saveGroup * 60, 67);
    }
    else
    {
        Gui::drawSelector(149 + (saveGroup - 4) * 60, 127);
    }

    Gui::text(i18n::localize("LOADER_INSTRUCTIONS_TOP_PRESENT"), 200, 8, FONT_SIZE_11, FONT_SIZE_11, COLOR_WHITE, TextPosX::CENTER, TextPosY::TOP);
}

void SaveLoadScreen::drawBottom() const
{
    Gui::backgroundBottom(true);
    Gui::sprite(ui_sheet_gameselector_savebox_idx, 22, 94);

    Gui::text(std::string(titleName(saveGroup)), 27, 26, FONT_SIZE_14, FONT_SIZE_14, COLOR_WHITE, TextPosX::LEFT, TextPosY::TOP);

    if (selectedSave > -1)
    {
        Gui::drawSolidRect(24, 96 + 17 * selectedSave, 174, 16, C2D_Color32(0x0f, 0x16, 0x59, 255));
    }

    int y = 97;
    for (int i = firstSave; i < firstSave + 6; i++)
    {
        if (i < (int)saves[saveGroup].size())
        {
            std::string save = saves[saveGroup][i].second.substr(0, saves[saveGroup][i].second.find_last_of('/'));
            save             = save.substr(save.find_last_of('/') + 1);
            save             = saves[saveGroup][i].first + save;
            Gui::text(save, 29, y, FONT_SIZE_11, FONT_SIZE_11, COLOR_WHITE, TextPosX::LEFT, TextPosY::TOP);
        }
        else
        {
            break;
        }
        y += 17;
    }

    if (selectedSave > 0 && firstSave > 0)
    {
        Gui::drawSolidRect(191, 102, 4, 5, C2D_Color32(0x0f, 0x16, 0x59, 255));
        Gui::drawSolidTriangle(189, 102, 197, 102, 193, 97, C2D_Color32(0x0f, 0x16, 0x59, 255));
    }

    if (selectedSave < 5 && (size_t)firstSave + 5 < saves[saveGroup].size() - 1)
    {
        Gui::drawSolidRect(191, 186, 4, 5, C2D_Color32(0x0f, 0x16, 0x59, 255));
        Gui::drawSolidTriangle(189, 191, 197, 191, 193, 196, C2D_Color32(0x0f, 0x16, 0x59, 255));
    }

    Gui::text(i18n::localize("LOADER_LOAD"), 248, 113, FONT_SIZE_14, FONT_SIZE_14, COLOR_WHITE, TextPosX::CENTER, TextPosY::TOP);
    Gui::text(i18n::localize("LOADER_WIRELESS"), 248, 163, FONT_SIZE_14, FONT_SIZE_14, COLOR_WHITE, TextPosX::CENTER, TextPosY::TOP);

    Gui::text(i18n::localize("LOADER_INSTRUCTIONS_BOTTOM"), 160, 223, FONT_SIZE_11, FONT_SIZE_11, COLOR_WHITE, TextPosX::CENTER, TextPosY::TOP);

    Gui::drawSolidRect(245, 23, 48, 48, COLOR_BLACK);
    Gui::drawSolidRect(243, 21, 52, 52, C2D_Color32(15, 22, 89, 255));
    Gui::sprite(ui_sheet_gameselector_unknown_idx, 245, 23);
}

void SaveLoadScreen::update(touchPosition* touch)
{
    u32 downKeys = hidKeysDown();
    if (selectedGroup)
    {
        if (downKeys & KEY_B)
        {
            selectedGroup = false;
            selectedSave  = -1;
            firstSave     = 0;
        }
        if (downKeys & KEY_A)
        {
            loadSave();
            return;
        }
        if (downKeys & KEY_X)
        {
            Gui::setScreen(std::make_unique<ConfigScreen>());
        }
        if (downKeys & KEY_DOWN)
        {
            if (selectedSave == 4)
            {
                if (firstSave + 5 < (int)saves[saveGroup].size() - 1)
                {
                    firstSave++;
                }
                else if (firstSave + selectedSave < (int)saves[saveGroup].size() - 1)
                {
                    selectedSave++;
                }
            }
            else
            {
                if (firstSave + selectedSave < (int)saves[saveGroup].size() - 1)
                {
                    selectedSave++;
                }
            }
        }
        if (downKeys & KEY_UP)
        {
            if (selectedSave == 1)
            {
                if (firstSave > 0)
                {
                    firstSave--;
                }
                else
                {
                    selectedSave--;
                }
            }
            else if (selectedSave != 0)
            {
                selectedSave--;
            }
        }
        for (auto& button : buttons)
        {
            button->update(touch);
        }
    }
    else
    {
        if (downKeys & KEY_DOWN)
        {
            if (saveGroup == -1)
            {
                saveGroup = 4;
            }
            else if (saveGroup < 4)
            {
                saveGroup += 4;
            }
            else
            {
                saveGroup -= 4;
            }
        }
        else if (downKeys & KEY_UP)
        {
            if (saveGroup == -1)
            {
                saveGroup = 0;
            }
            else if (saveGroup < 4)
            {
                saveGroup += 4;
            }
            else
            {
                saveGroup -= 4;
            }
        }
        else if (downKeys & KEY_RIGHT)
        {
            if (saveGroup == -1)
            {
                saveGroup = 0;
            }
            else if (saveGroup % 4 == 3)
            {
                saveGroup = -1;
            }
            else
            {
                saveGroup++;
            }
        }
        else if (downKeys & KEY_LEFT)
        {
            if (saveGroup == -1)
            {
                saveGroup = 3;
            }
            else if (saveGroup % 4 == 0)
            {
                saveGroup = -1;
            }
            else
            {
                saveGroup--;
            }
        }
        else if (downKeys & KEY_Y)
        {
            Gui::screenBack();
            Gui::setScreen(std::make_unique<TitleLoadScreen>());
            return;
        }
        if (downKeys & KEY_A)
        {
            if (saves[saveGroup].size() != 0)
            {
                selectedGroup = true;
                selectedSave  = 0;
            }
        }
    }
    if (downKeys & KEY_X)
    {
        Gui::setScreen(std::make_unique<ConfigScreen>());
    }
}

bool SaveLoadScreen::loadSave()
{
    if (TitleLoader::load(nullptr, saves[saveGroup][selectedSave + firstSave].second))
    {
        Gui::setScreen(std::make_unique<MainMenu>());
        return true;
    }
    return false;
}

constexpr std::string_view SaveLoadScreen::titleName(int index)
{
    if (index < 4)
    {
        return dsNames[index + 1];
    }
    else
    {
        return ctrNames[index - 4];
    }
}

bool SaveLoadScreen::setSelectedSave(int i)
{
    if (i == 5)
    {
        if (firstSave + 5 < (int)saves[saveGroup].size() - 1)
        {
            firstSave++;
            selectedSave = 4;
        }
        else if (firstSave + 5 < (int)saves[saveGroup].size())
        {
            selectedSave = 5;
        }
    }
    else if (i == 0 && firstSave != 0)
    {
        firstSave--;
        selectedSave = 1;
    }
    else if (firstSave + i < (int)saves[saveGroup].size())
    {
        selectedSave = i;
    }
    return false;
}
