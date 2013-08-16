/***************************************************************************
                              qgswfsserver.h
                              -------------------
  begin                : February 27, 2012
  copyright            : (C) 2012 by René-Luc D'Hont
  email                : rldhont at 3liz dot com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSWFSSERVER_H
#define QGSWFSSERVER_H

#include <QDomDocument>
#include <QMap>
#include <QString>
#include <map>

class QgsCoordinateReferenceSystem;
class QgsComposerLayerItem;
class QgsComposerLegendItem;
class QgsComposition;
class QgsMapLayer;
class QgsMapRenderer;
class QgsPoint;
class QgsRasterLayer;
class QgsConfigParser;
class QgsVectorLayer;
class QgsCoordinateReferenceSystem;
class QgsField;
class QgsFeature;
class QgsGeometry;
class QgsSymbol;
class QgsRequestHandler;
class QFile;
class QFont;
class QImage;
class QPaintDevice;
class QPainter;

/**This class handles all the wms server requests. The parameters and values have to be passed in the form of
a map<QString, QString>. This map is usually generated by a subclass of QgsWMSRequestHandler, which makes QgsWFSServer
independent from any server side technology*/

class QgsWFSServer
{
  public:
    /**Constructor. Takes parameter map and a pointer to a renderer object (does not take ownership)*/
    QgsWFSServer( QMap<QString, QString> parameters );
    ~QgsWFSServer();
    /**Returns an XML file with the capabilities description (as described in the WFS specs)*/
    QDomDocument getCapabilities();

    /**Returns an XML file with the describe feature type (as described in the WFS specs)*/
    QDomDocument describeFeatureType();

    /**Creates a document that describes the result of the getFeature request.
       @return 0 in case of success*/
    int getFeature( QgsRequestHandler& request, const QString& format );

    /**Sets configuration parser for administration settings. Does not take ownership*/
    void setAdminConfigParser( QgsConfigParser* parser ) { mConfigParser = parser; }

  private:
    /**Don't use the default constructor*/
    QgsWFSServer();

    /**Map containing the WMS parameters*/
    QMap<QString, QString> mParameterMap;
    QgsConfigParser* mConfigParser;
    QString mTypeName;
    bool mWithGeom;

  protected:

    void startGetFeature( QgsRequestHandler& request, const QString& format );
    void sendGetFeature( QgsRequestHandler& request, const QString& format, QgsFeature* feat, int featIdx, QgsCoordinateReferenceSystem& crs, QMap< int, QgsField > fields, QSet<QString> hiddenAttributes );
    void endGetFeature( QgsRequestHandler& request, const QString& format );

    //methods to write GeoJSON
    QString createFeatureGeoJSON( QgsFeature* feat, QgsCoordinateReferenceSystem& crs, QMap< int, QgsField > fields, QSet<QString> hiddenAttributes ) /*const*/;

    //methods to write GML2
    QDomElement createFeatureElem( QgsFeature* feat, QDomDocument& doc, QgsCoordinateReferenceSystem& crs, QMap< int, QgsField > fields, QSet<QString> hiddenAttributes ) /*const*/;

    QDomElement createGeometryElem( QgsGeometry* g, QDomDocument& doc ) /*const*/;
    QDomElement createLineStringElem( QgsGeometry* geom, QDomDocument& doc ) const;
    QDomElement createMultiLineStringElem( QgsGeometry* geom, QDomDocument& doc ) const;
    QDomElement createPointElem( QgsGeometry* geom, QDomDocument& doc ) const;
    QDomElement createMultiPointElem( QgsGeometry* geom, QDomDocument& doc ) const;
    QDomElement createPolygonElem( QgsGeometry* geom, QDomDocument& doc ) const;
    QDomElement createMultiPolygonElem( QgsGeometry* geom, QDomDocument& doc ) const;

    /**Create a GML coordinate string from a point list.
      @param points list of data points
      @param coordString out: GML coord string
      @return 0 in case of success*/
    QDomElement createCoordinateElem( const QVector<QgsPoint> points, QDomDocument& doc ) const;
};

#endif