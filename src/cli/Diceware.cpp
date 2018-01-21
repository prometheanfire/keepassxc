/*
 *  Copyright (C) 2018 KeePassXC Team <team@keepassxc.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdlib>
#include <stdio.h>

#include "Diceware.h"

#include <QCommandLineParser>
#include <QTextStream>

#include "core/PassphraseGenerator.h"

Diceware::Diceware()
{
    this->name = QString("diceware");
    this->description = QObject::tr("Generate a new random password.");
}

Diceware::~Diceware()
{
}

int Diceware::execute(QStringList arguments)
{
    QTextStream inputTextStream(stdin, QIODevice::ReadOnly);
    QTextStream outputTextStream(stdout, QIODevice::WriteOnly);

    QCommandLineParser parser;
    parser.setApplicationDescription(this->description);
    QCommandLineOption wordlistFile(QStringList() << "w"
                                             << "word-list",
                               QObject::tr("Wordlist fot the diceware generator.\n[Default: EFF English]"),
                               QObject::tr("path"));
    parser.addOption(wordlistFile);
    parser.addPositionalArgument("words", QObject::tr("Word count for the diceware generator."));
    parser.process(arguments);

    const QStringList args = parser.positionalArguments();
    if (args.size() != 1) {
        outputTextStream << parser.helpText().replace("keepassxc-cli", "keepassxc-cli diceware");
        return EXIT_FAILURE;
    }

    PassphraseGenerator dicewareGenerator;

    int words = args.at(0).toInt();
    dicewareGenerator.setWordCount(words);

    if (!parser.value(wordlistFile).isEmpty()) {
        dicewareGenerator.setWordList(parser.value(wordlistFile));
    } else {
        dicewareGenerator.setDefaultWordList();
    }

    if (!dicewareGenerator.isValid()) {
        outputTextStream << parser.helpText().replace("keepassxc-cli", "keepassxc-cli diceware");
        return EXIT_FAILURE;
    }
    
    QString password = dicewareGenerator.generatePassphrase();
    outputTextStream << password << endl;

    return EXIT_SUCCESS;
}