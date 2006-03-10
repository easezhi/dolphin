/* This file is part of the KDE projects
   Copyright (C) 2005 David Faure <faure@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License version 2 as published by the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "konqmimedata.h"
#include <qmimedata.h>
#include <kdebug.h>

void KonqMimeData::populateMimeData( QMimeData* mimeData,
                                     const KUrl::List& kdeURLs,
                                     const KUrl::List& mostLocalURLs,
                                     bool cut )
{
    mostLocalURLs.populateMimeData( mimeData );

    if ( !kdeURLs.isEmpty() )
    {
        QMimeData tmpMimeData;
        kdeURLs.populateMimeData(&tmpMimeData);
        mimeData->setData("application/x-kde-urilist",tmpMimeData.data("text/uri-list"));
    }

    QByteArray cutSelectionData = cut ? "1" : "0";
    mimeData->setData( "application/x-kde-cutselection", cutSelectionData );

    // for compatibility reasons
    QString application_x_qiconlist;
    int items=qMax(kdeURLs.count(),mostLocalURLs.count());
    for (int i=0;i<items;i++) {
	int offset=i*16;
	QString tmp("%1$@@$%2$@@$32$@@$32$@@$%3$@@$%4$@@$32$@@$16$@@$no data$@@$");
	tmp=tmp.arg(offset).arg(offset).arg(offset).arg(offset+40);
	application_x_qiconlist+=tmp;
    }
    mimeData->setData("application/x-qiconlist",application_x_qiconlist.toLatin1());
    kDebug(1203)<<"setting application/x-qiconlist to "<<application_x_qiconlist<<endl;

}

bool KonqMimeData::decodeIsCutSelection( const QMimeData *mimeData )
{
    QByteArray a = mimeData->data( "application/x-kde-cutselection" );
    if ( a.isEmpty() )
        return false;
    else
    {
        kDebug(1203) << "KonqDrag::decodeIsCutSelection : a=" << a << endl;
        return (a.at(0) == '1'); // true if 1
    }
}
