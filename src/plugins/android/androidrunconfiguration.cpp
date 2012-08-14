/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2012 BogDan Vatra <bog_dan_ro@yahoo.com>
**
** Contact: http://www.qt-project.org/
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**************************************************************************/

#include "androidrunconfiguration.h"
#include "androiddeploystep.h"
#include "androidglobal.h"
#include "androidtoolchain.h"
#include "androidmanager.h"

#include <projectexplorer/profileinformation.h>
#include <projectexplorer/target.h>
#include <qtsupport/qtoutputformatter.h>
#include <qtsupport/qtprofileinformation.h>

#include <utils/qtcassert.h>

using namespace ProjectExplorer;

namespace Android {
namespace Internal {

AndroidRunConfiguration::AndroidRunConfiguration(Target *parent, Core::Id id, const QString &path)
    : RunConfiguration(parent, id)
    , m_proFilePath(path)
{
    init();
}

AndroidRunConfiguration::AndroidRunConfiguration(Target *parent, AndroidRunConfiguration *source)
    : RunConfiguration(parent, source)
    , m_proFilePath(source->m_proFilePath)
{
    init();
}

void AndroidRunConfiguration::init()
{
    setDefaultDisplayName(defaultDisplayName());
}

QWidget *AndroidRunConfiguration::createConfigurationWidget()
{
    return 0;// no special running configurations
}

Utils::OutputFormatter *AndroidRunConfiguration::createOutputFormatter() const
{
    return new QtSupport::QtOutputFormatter(target()->project());
}

QString AndroidRunConfiguration::defaultDisplayName()
{
    return tr("Run on Android device");
}

AndroidConfig AndroidRunConfiguration::config() const
{
    return AndroidConfigurations::instance().config();
}

const Utils::FileName AndroidRunConfiguration::gdbCmd() const
{
    ToolChain *tc = ToolChainProfileInformation::toolChain(target()->profile());
    if (!tc)
        return Utils::FileName();
    return AndroidConfigurations::instance().gdbPath(tc->targetAbi().architecture());
}

AndroidDeployStep *AndroidRunConfiguration::deployStep() const
{
    AndroidDeployStep * const step
        = AndroidGlobal::buildStep<AndroidDeployStep>(target()->activeDeployConfiguration());
    Q_ASSERT_X(step, Q_FUNC_INFO,
        "Impossible: Android build configuration without deploy step.");
    return step;
}


const QString AndroidRunConfiguration::remoteChannel() const
{
    return QLatin1String(":5039");
}

const QString AndroidRunConfiguration::dumperLib() const
{
    QtSupport::BaseQtVersion *version = QtSupport::QtProfileInformation::qtVersion(target()->profile());
    if (!version)
        return QString();
    return version->gdbDebuggingHelperLibrary();
}

QString AndroidRunConfiguration::proFilePath() const
{
    return m_proFilePath;
}

} // namespace Internal
} // namespace Android
