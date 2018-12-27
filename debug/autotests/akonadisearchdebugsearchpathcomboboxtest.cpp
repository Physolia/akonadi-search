/*
  Copyright (c) 2014-2019 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "akonadisearchdebugsearchpathcomboboxtest.h"
#include "../akonadisearchdebugsearchpathcombobox.h"
#include <qtest.h>

AkonadiSearchDebugSearchPathComboBoxTest::AkonadiSearchDebugSearchPathComboBoxTest(QObject *parent)
    : QObject(parent)
{

}

AkonadiSearchDebugSearchPathComboBoxTest::~AkonadiSearchDebugSearchPathComboBoxTest()
{

}

void AkonadiSearchDebugSearchPathComboBoxTest::shouldHaveDefaultValue()
{
    Akonadi::Search::AkonadiSearchDebugSearchPathComboBox combox;
    QVERIFY(combox.count() > 0);
}

void AkonadiSearchDebugSearchPathComboBoxTest::shouldReturnPath()
{
    Akonadi::Search::AkonadiSearchDebugSearchPathComboBox combox;
    QVERIFY(!combox.searchPath().isEmpty());
}

void AkonadiSearchDebugSearchPathComboBoxTest::shouldReturnCorrectSearchPath()
{
    Akonadi::Search::AkonadiSearchDebugSearchPathComboBox combox;
    QString path = combox.pathFromEnum(Akonadi::Search::AkonadiSearchDebugSearchPathComboBox::Contacts);
    QCOMPARE(combox.searchPath(), path);
}

void AkonadiSearchDebugSearchPathComboBoxTest::shouldSelectCorrectType()
{
    Akonadi::Search::AkonadiSearchDebugSearchPathComboBox combox;
    QString path = combox.pathFromEnum(Akonadi::Search::AkonadiSearchDebugSearchPathComboBox::ContactCompleter);
    combox.setSearchType(Akonadi::Search::AkonadiSearchDebugSearchPathComboBox::ContactCompleter);
    QCOMPARE(combox.searchPath(), path);
    path = combox.pathFromEnum(Akonadi::Search::AkonadiSearchDebugSearchPathComboBox::Emails);
    combox.setSearchType(Akonadi::Search::AkonadiSearchDebugSearchPathComboBox::Emails);
    QCOMPARE(combox.searchPath(), path);

}

QTEST_MAIN(AkonadiSearchDebugSearchPathComboBoxTest)
