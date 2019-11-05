#include "LibKML.h"
#include "kml/dom.h"
#include "kml/convenience/convenience.h"
#include "kml/base/date_time.h"
#include <time.h>
#include <iostream>
#include <fstream>
#include <stdlib.h> 
#include "kml/engine.h"
#include "kml/base/file.h"

CLibKML::CLibKML()
{
}

GPX_model::retCode_e CLibKML::load(ifstream *fp, GPX_model *gpxm, bool overwriteMetadata)
{
    GPX_model::retCode_e ret = GPX_model::GPXM_OK;
    
    return ret;
}

static const char kDotIcon[] =
    "http://maps.google.com/mapfiles/kml/shapes/shaded_dot.png";
const char kStartPointIcon[] = "http://files.2bulu.com/f/d1?downParams=n%2BVE0NaFmzwndk%2BZ75NU9A%3D%3D%0A";
const char kStopPointIcon[] = "http://files.2bulu.com/f/d1?downParams=0Q3GJ5o%2FHpsr0OBxohfNOA%3D%3D%0A";

static kmldom::IconStylePtr CreateIconStyle(double scale) {
  kmldom::KmlFactory* kml_factory = kmldom::KmlFactory::GetFactory();
  kmldom::IconStyleIconPtr icon = kml_factory->CreateIconStyleIcon();
  icon->set_href(kDotIcon);
  kmldom::IconStylePtr icon_style = kml_factory->CreateIconStyle();
  icon_style->set_icon(icon);
  icon_style->set_scale(scale);
  return icon_style;
}

static kmldom::LabelStylePtr CreateLabelStyle(double scale) {
  kmldom::LabelStylePtr label_style = kmldom::KmlFactory::GetFactory()->CreateLabelStyle();
  label_style->set_scale(scale);
  return label_style;
}

kmldom::LineStylePtr CreateLineStyle(int nWidth = 5, kmlbase::Color32 col = kmlbase::Color32(255, 0, 0, 255)){
    kmldom::LineStylePtr lineStyle = kmldom::KmlFactory::GetFactory()->CreateLineStyle();
    lineStyle->set_color(col);
    lineStyle->set_width(nWidth);
    return lineStyle;            
}

kmldom::StylePtr CreateIconStyle(std::string szId, std::string szIconUrl){
    kmldom::KmlFactory* kml_factory = kmldom::KmlFactory::GetFactory();
    kmldom::StylePtr style = kml_factory->CreateStyle();
    style->set_id(szId);
    kmldom::IconStyleIconPtr icon = kml_factory->CreateIconStyleIcon();
    icon->set_href(szIconUrl);
    kmldom::IconStylePtr icon_style = kml_factory->CreateIconStyle();
    icon_style->set_icon(icon);
    style->set_iconstyle(icon_style);
    return style;
}

static kmldom::PairPtr CreatePair(int style_state, double icon_scale) {
  kmldom::KmlFactory* kml_factory = kmldom::KmlFactory::GetFactory();
  kmldom::PairPtr pair = kml_factory->CreatePair();
  pair->set_key(style_state);
  kmldom::StylePtr style = kml_factory->CreateStyle();
  style->set_iconstyle(CreateIconStyle(icon_scale));
  // Hide the label in normal style state, visible in highlight.
  style->set_labelstyle(CreateLabelStyle(
      style_state == kmldom::STYLESTATE_NORMAL ? 0 : 1 ));
  
  if(style_state == kmldom::STYLESTATE_NORMAL)
      style->set_linestyle(CreateLineStyle());
  else
      style->set_linestyle(CreateLineStyle(8, kmlbase::Color32(255, 0, 255, 255)));
  pair->set_styleselector(style);
  return pair;
}

static kmldom::StyleMapPtr CreateStyleMap(const char* id) {
  kmldom::KmlFactory* kml_factory = kmldom::KmlFactory::GetFactory();
  kmldom::StyleMapPtr style_map = kml_factory->CreateStyleMap();
  style_map->set_id(id);
  style_map->add_pair(CreatePair(kmldom::STYLESTATE_NORMAL, 1.1));
  style_map->add_pair(CreatePair(kmldom::STYLESTATE_HIGHLIGHT, 1.3));
  return style_map;
}

GPX_model::retCode_e CLibKML::save(ofstream *fp, const GPX_model *gpxm)
{
    std::cout << "CLibKML::save" << std::endl;
    GPX_model::retCode_e ret = GPX_model::GPXM_OK;
    
    kmldom::KmlFactory* pKmlFactory = kmldom::KmlFactory::GetFactory();
    if(!pKmlFactory)
        return GPX_model::GPXM_ERR_FAILED;
    // Create a <Document> to hand to the GPX parser.
    kmldom::DocumentPtr pDocument = pKmlFactory->CreateDocument();
    if(!pDocument)
        return GPX_model::GPXM_ERR_FAILED;
    pDocument->set_name(gpxm->metadata.name);
    kmldom::AtomAuthorPtr pAuthor = pKmlFactory->CreateAtomAuthor();
    if(!gpxm->metadata.author.name.empty())
        pAuthor->set_name(gpxm->metadata.author.name);
    if(!gpxm->metadata.author.link.href.empty())
        pAuthor->set_uri(gpxm->metadata.author.link.href);
    if(!gpxm->metadata.author.email.id.empty()
            || !gpxm->metadata.author.email.domain.empty())
        pAuthor->set_email(gpxm->metadata.author.email.id
                           + "@"
                           + gpxm->metadata.author.email.domain);
    pDocument->set_atomauthor(pAuthor);
    if(!gpxm->metadata.links.empty())
    {
        kmldom::AtomLinkPtr pLink = pKmlFactory->CreateAtomLink();
        pLink->set_href(gpxm->metadata.links[0].href);
        pLink->set_title(gpxm->metadata.links[0].text);
        pLink->set_type(gpxm->metadata.links[0].type);
        pDocument->set_atomlink(pLink);
    }
    pDocument->set_description(gpxm->metadata.desc);
    pDocument->add_styleselector(CreateIconStyle("StartPointIconStyle", kStartPointIcon));
    pDocument->add_styleselector(CreateIconStyle("StopPointIconStyle", kStopPointIcon));
    
    const char* kStyleMapId = "style-map";
    pDocument->add_styleselector(CreateStyleMap(kStyleMapId));
      
    int num = 0;
    std::vector<GPX_trkType>::const_iterator it;
    for(it = gpxm->trk.begin(); it != gpxm->trk.end(); it++)
    {
        kmldom::FolderPtr pFolderTrack = pKmlFactory->CreateFolder();
        if(!pFolderTrack)
            continue;
        pDocument->add_feature(pFolderTrack);
        char szNum[36];
        itoa(num++, &szNum[0], 10);
        pFolderTrack->set_id(szNum);
        pFolderTrack->set_name(std::string("Track")); // + szNum);
        
        int trackNum = 0;
        std::vector<GPX_trksegType>::const_iterator itSeg;
        for(itSeg = it->trkseg.begin(); itSeg != it->trkseg.end(); itSeg++)
        {
            kmldom::FolderPtr pFolderTrackSeg = pKmlFactory->CreateFolder();
            if(!pFolderTrackSeg)
                continue;
            pFolderTrack->add_feature(pFolderTrackSeg);
            pFolderTrackSeg->set_name("Track segment mark");
            
            GPX_wptType itWptStart = itSeg->trkpt[0];
            kmldom::PlacemarkPtr pPlacemarkStart = pKmlFactory->CreatePlacemark();
            if(!pPlacemarkStart)
               continue;
            pFolderTrackSeg->add_feature(pPlacemarkStart);
            pPlacemarkStart->set_id("Start point");
            pPlacemarkStart->set_name("Start Point");
            pPlacemarkStart->set_styleurl("#StartPointIconStyle");
            kmlbase::Vec3 v(itWptStart.longitude, itWptStart.latitude, itWptStart.altitude);
            kmldom::PointPtr pStartPoint = kmlconvenience::CreatePointFromVec3(v);
            if(pStartPoint)
               pPlacemarkStart->set_geometry(pStartPoint);
            kmldom::TimeStampPtr timeStampStart = pKmlFactory->CreateTimeStamp();
            if(timeStampStart)
            {
                tm* t = gmtime(&itWptStart.timestamp);
                char buf[32];
                strftime(buf, sizeof(buf), "%H:%M:%ST%Y-%m-%dZ", t);
                timeStampStart->set_when(buf);
                pPlacemarkStart->set_timeprimitive(timeStampStart);
            }
            
            GPX_wptType itWptEnd = itSeg->trkpt[itSeg->stats.points - 1];
            kmldom::PlacemarkPtr pPlacemarkEnd = pKmlFactory->CreatePlacemark();
            if(!pPlacemarkEnd)
                continue;
            pFolderTrackSeg->add_feature(pPlacemarkEnd);
            pPlacemarkEnd->set_id("End point");
            pPlacemarkEnd->set_name("End point");
            pPlacemarkEnd->set_styleurl("#StopPointIconStyle");
            kmlbase::Vec3 vEnd(itWptEnd.longitude, itWptEnd.latitude, itWptEnd.altitude);
            kmldom::PointPtr pEndPoint = kmlconvenience::CreatePointFromVec3(vEnd);
            if(pEndPoint)
                pPlacemarkEnd->set_geometry(pEndPoint);
            kmldom::TimeStampPtr timeStampEnd = pKmlFactory->CreateTimeStamp();
            if(timeStampEnd)
            {
                tm* t = gmtime(&itWptEnd.timestamp);
                char buf[32];
                strftime(buf, sizeof(buf), "%H:%M:%ST%Y-%m-%dZ", t);
                timeStampEnd->set_when(buf);
                pPlacemarkEnd->set_timeprimitive(timeStampEnd);
            }
            
            //Track segment  
            //kmldom::FolderPtr pFolderTrackSeg = pKmlFactory->CreateFolder();
            //if(!pFolderTrackSeg)
            //    continue;
            //pFolderTrack->add_feature(pFolderTrackSeg);
            kmldom::PlacemarkPtr pPlacemarkTrack = pKmlFactory->CreatePlacemark();
            if(!pPlacemarkTrack)
                continue;
            pFolderTrackSeg->add_feature(pPlacemarkTrack);
            pPlacemarkTrack->set_styleurl(string("#") + kStyleMapId);
            char szTrackNum[36];
            itoa(trackNum++, &szTrackNum[0], 10);
            pFolderTrackSeg->set_id(szTrackNum);
            pFolderTrackSeg->set_name(std::string("Track segment")); // + szTrackNum);
            kmldom::GxTrackPtr pTrack = pKmlFactory->CreateGxTrack();
            if(!pTrack)
                continue;
            pPlacemarkTrack->set_geometry(pTrack);
                        
            std::vector<GPX_wptType>::const_iterator itWpt;
            for(itWpt = itSeg->trkpt.begin(); itWpt != itSeg->trkpt.end();
                itWpt++)
            {
                kmlbase::Vec3 v(itWpt->longitude, itWpt->latitude, itWpt->altitude);
                pTrack->add_gx_coord(v);
                tm* t = gmtime(&itWpt->timestamp);
                char buf[32];
                strftime(buf, sizeof(buf), "%H:%M:%ST%Y-%m-%dZ", t);
                pTrack->add_when(buf);
            }
      
        }
    }
    
    // Put the <Document> in a <kml> element and write everything out to the
    // supplied pathname.
    kmldom::KmlPtr kml = pKmlFactory->CreateKml();
    kml->set_feature(pDocument);
    std::string str = kmldom::SerializePretty(kml);
    std::string errors;
    kmlengine::KmlFilePtr kml_file = kmlengine::KmlFile::CreateFromParse(str, &errors);
    kml_file->SerializeToOstream(fp);
    return ret;
}
