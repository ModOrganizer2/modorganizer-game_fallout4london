#include "fo4londonbsainvalidation.h"

#include "dummybsa.h"
#include "iplugingame.h"
#include "iprofile.h"
#include "registry.h"
#include <imoinfo.h>
#include <utility.h>

Fallout4LondonBSAInvalidation::Fallout4LondonBSAInvalidation(MOBase::DataArchives* dataArchives,
                                                 MOBase::IPluginGame const* game)
    : GamebryoBSAInvalidation(dataArchives, "Fallout4Custom.ini", game)
{
  m_IniFileName = "Fallout4Custom.ini";
  m_Game        = game;
}

bool Fallout4LondonBSAInvalidation::isInvalidationBSA(const QString& bsaName)
{
  return false;
}

QString Fallout4LondonBSAInvalidation::invalidationBSAName() const
{
  return "";
}

unsigned long Fallout4LondonBSAInvalidation::bsaVersion() const
{
  return 0x68;
}

bool Fallout4LondonBSAInvalidation::prepareProfile(MOBase::IProfile* profile)
{
  bool dirty          = false;
  QString basePath    = profile->localSettingsEnabled()
                            ? profile->absolutePath()
                            : m_Game->documentsDirectory().absolutePath();
  QString iniFilePath = basePath + "/" + m_IniFileName;
  WCHAR setting[MAX_PATH];

  if (profile->invalidationActive(nullptr)) {
    // write bInvalidateOlderFiles = 1, if needed
    if (!::GetPrivateProfileStringW(L"Archive", L"bInvalidateOlderFiles", L"0", setting,
                                    MAX_PATH, iniFilePath.toStdWString().c_str()) ||
        wcstol(setting, nullptr, 10) != 1) {
      dirty = true;
      if (!MOBase::WriteRegistryValue(L"Archive", L"bInvalidateOlderFiles", L"1",
                                      iniFilePath.toStdWString().c_str())) {
        qWarning("failed to override data directory in \"%s\"",
                 qUtf8Printable(m_IniFileName));
      }
    }
    if (!::GetPrivateProfileStringW(L"Archive", L"sResourceDataDirsFinal", L"STRINGS\\",
                                    setting, MAX_PATH,
                                    iniFilePath.toStdWString().c_str()) ||
        wcscmp(setting, L"") != 0) {
      dirty = true;
      if (!MOBase::WriteRegistryValue(L"Archive", L"sResourceDataDirsFinal", L"",
                                      iniFilePath.toStdWString().c_str())) {
        qWarning("failed to override data directory in \"%s\"",
                 qUtf8Printable(m_IniFileName));
      }
    }
  }

  return dirty;
}
