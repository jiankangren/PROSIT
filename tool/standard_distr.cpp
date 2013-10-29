#include "distr_factory.hpp"
#include "standard_distr.hpp"

namespace StandardDistributions {
  DistrFactory::DistrParameters * FileDistrBuilder::parse_parameters(XMLElement * distElement) throw(Exc) {
    std::string name;
    int sz;    
    XMLElement * internal;
    if(!(internal= distElement->FirstChildElement("fileName"))) 
      EXC_PRINT_2("computation time pmf undefined for task", name);
    name = internal->GetText();
    if((internal= distElement->FirstChildElement("size"))) {
      internal->QueryIntText(&sz);
      return new FileDistrParameters(name, sz);
    };
    return new FileDistrParameters(name);
  };
  auto_ptr<pmf> FileDistrBuilder::create_instance(DistrFactory::DistrParameters * t) throw (Exc) {
    FileDistrParameters *pt;
    if (!(pt = dynamic_cast<FileDistrParameters*>(t)))
      EXC_PRINT("worng parameter type");

    auto_ptr<pmf> c (new pmf (pt->size,0));
    c->load(pt->fileName);
    return c;
  };
  
  DistrFactory::DistrParameters * SyntheticDistrBuilder::parse_parameters(XMLElement * distrElement) throw (Exc) {
    int cmin;
    int cmax;
    int step;
    int size;
    bool dump=false;
    std::string dump_file;

    XMLElement * internal;
    
    if(!(internal = distrElement->FirstChildElement("cmin"))) 
      EXC_PRINT("cmin missing");
    internal->QueryIntText(&cmin);
    
    if(!(internal = distrElement->FirstChildElement("cmax"))) 
      EXC_PRINT("cmax missing");
    internal->QueryIntText(&cmax);
    
    if(!(internal = distrElement->FirstChildElement("step"))) 
      EXC_PRINT("step missing");

    internal->QueryIntText(&step);
    if(!(internal = distrElement->FirstChildElement("size"))) 
      EXC_PRINT("size missing");
    internal->QueryIntText(&size);
    if ((internal = distrElement->FirstChildElement("dump"))) 
      dump_file = internal->GetText();
  
    return new SyntheticDistrParameters(cmin, cmax, step,size, dump, dump_file);
  };
						
  DistrFactory::DistrParameters * UniformDistrBuilder::parse_parameters(XMLElement * distrElement) throw (Exc) {
    return SyntheticDistrBuilder::parse_parameters(distrElement);
  }


    auto_ptr<pmf> UniformDistrBuilder::create_instance(DistrFactory::DistrParameters * t) throw (Exc) {
    SyntheticDistrParameters *pt;
    if (!(pt = dynamic_cast<SyntheticDistrParameters*>(t))) 
      EXC_PRINT("wrong parameter type");
    auto_ptr<pmf> c (new pmf (pt->size,0));
    if (pt->cmax < pt->cmin) 
      EXC_PRINT("cmax smaller than cmin");

    if ((pt->cmax - pt->cmin)%pt->step != 0) 
      EXC_PRINT("step has to be an integer submultiple of the interval");
    int samples = (pt->cmax-pt->cmin)/pt->step+1;
    
    
    double prob = 1.0/double(samples);
    for (int h = pt->cmin; h<=pt->cmax; h+= pt->step)
      c->set(h,prob);
    return c;
  };
  DistrFactory::DistrParameters * BetaDistrBuilder::parse_parameters(XMLElement * distrElement) throw (Exc) {
    DistrFactory::DistrParameters* pt = SyntheticDistrBuilder::parse_parameters(distrElement);
    SyntheticDistrParameters *p = dynamic_cast<SyntheticDistrParameters*>(pt);
    double a;
    double b;
    
    XMLElement * internal;
       
    if(!(internal = distrElement->FirstChildElement("a")))
      EXC_PRINT("a parameter missing");
    internal->QueryDoubleText(&a);
 
    if(!(internal = distrElement->FirstChildElement("b")))
      EXC_PRINT("b parameter missing");
    internal->QueryDoubleText(&b);
    
    if((a <=1.0) || (b<=1.0)) {
      delete p;
      EXC_PRINT("a and b parameters have to be greater than 1.0");
    }
    BetaDistrParameters * t = new BetaDistrParameters(p->cmin, p->cmax, p->step, p->size, a, b, p->dump, p->dump_file);
    delete p;
    return t;
  }

  auto_ptr<pmf> BetaDistrBuilder::create_instance(DistrFactory::DistrParameters * t) throw (Exc) {
    BetaDistrParameters *pt;
    if (!(pt = dynamic_cast<BetaDistrParameters*>(t)))
      EXC_PRINT("wrong parameters type");
    

    auto_ptr<pmf> c (new pmf (pt->size,0));
    if ((pt->cmax - pt->cmin)%pt->step != 0)
      EXC_PRINT("step had to be an integer submultiple of the interval");
    if((pt->a<=0)||(pt->b<=0))
      EXC_PRINT("a and b parameters must be positive");

    double total_prob = 0;
    
    for (int h = pt->cmin; h<=pt->cmax; h+= pt->step) {
      double prob = pow(double(h-pt->cmin)/double(pt->cmax-pt->cmin),pt->a-1)*pow(1-(double(h-pt->cmin)/double(pt->cmax-pt->cmin)),pt->b-1);
      total_prob += prob;
      c->set(h,prob);
    }
    for (int h = pt->cmin; h<=pt->cmax; h+= pt->step)
      c->set(h,c->get(h)/total_prob);
  
    return c;
  };
  void init() {
    DistrFactory::distr_factory.register_type("file", new FileDistrBuilder() );
    DistrFactory::distr_factory.register_type("uniform", new UniformDistrBuilder() );
    DistrFactory::distr_factory.register_type("beta", new BetaDistrBuilder() );
  };
}
