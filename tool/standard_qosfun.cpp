#include "standard_qosfun.hpp"

namespace StandardQoSFun {
  LinearQoSFun::LinearQoSFun(double scaled, double pmind, double pmaxd) throw (Exc) : scale(scaled), pmin(pmind), pmax(pmaxd) {
    if ((pmaxd<pmind)||(scaled<0))
      EXC_PRINT("wrong initialisation parameters");
  };
  double LinearQoSFun::eval(double prob)  {
      if (prob <= pmin)
	return 0;
      if (prob >pmax)
	return scale*(pmax-pmin);
      return scale*(prob-pmin);
  };
  auto_ptr<QoSFun> LinearQoSFunBuilder::create_instance(QoSFactory::QoSFunParameters * t) throw (Exc) {
    LinearQoSFunParameters * p = dynamic_cast<LinearQoSFunParameters*>(t);
    if (!p)
      EXC_PRINT("wrong parameter type");
    if((p->pmin > p->pmax)||
       (p->pmin <0)||(p->pmin>1.0)||
       (p->pmax <0)||(p->pmax>1.0))
      EXC_PRINT("wrong probability limits");

    return auto_ptr<QoSFun>(new LinearQoSFun(p->scale, p->pmin, p->pmax));
  };
  QoSFactory::QoSFunParameters * LinearQoSFunBuilder::parse_parameters(XMLElement *qosfunel) throw (Exc) {
    XMLElement * internal;
    double scale;
    double pmin;
    double pmax;
    if(!(internal= qosfunel->FirstChildElement("scale"))) 
      EXC_PRINT("scale parameter undefined for qos function");
    internal->QueryDoubleText(&scale);
    if(!(internal= qosfunel->FirstChildElement("pmin"))) 
      EXC_PRINT("parameter pmin undefined for qos function");
    internal->QueryDoubleText(&pmin);
    if(!(internal= qosfunel->FirstChildElement("pmax"))) 
      EXC_PRINT("parameter pmax undefined for qos function");

    internal->QueryDoubleText(&pmax);
    return new LinearQoSFunParameters(scale,pmin,pmax);
  };
  
  auto_ptr<QoSFun> QuadraticQoSFunBuilder::create_instance(QoSFactory::QoSFunParameters * t) throw (Exc) {
    LinearQoSFunParameters * p = dynamic_cast<LinearQoSFunParameters*>(t);
    if (!p)
      EXC_PRINT("wrong parameter type");
    if((p->pmin > p->pmax)||
       (p->pmin <0)||(p->pmin>1.0)||
       (p->pmax <0)||(p->pmax>1.0))
      EXC_PRINT("wrong probability limits");

    return auto_ptr<QoSFun>(new QuadraticQoSFun(p->scale, p->pmin, p->pmax));
  };
  QuadraticQoSFun::QuadraticQoSFun(double scaled, double pmind, double pmaxd) throw (Exc) : scale(scaled), pmin(pmind), pmax(pmaxd) {
    if ((pmaxd<pmind)||(scaled<0))
      EXC_PRINT("wrong initialisation parameters");
  };
  double QuadraticQoSFun::eval(double prob)  {
      if (prob <= pmin)
	return 0;
      if (prob >pmax)
	return scale*(pmax-pmin)*(pmax-pmin);
      return scale*(prob-pmin)*(prob-pmin);
  };


  void init() {
    QoSFactory::register_qosfun_type("linear",new(LinearQoSFunBuilder));
    QoSFactory::register_qosfun_type("quadratic",new(QuadraticQoSFunBuilder));
  };

};
