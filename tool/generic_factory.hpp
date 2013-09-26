/*
 * Template class to create generic factories
 *
 * Copyright (C) 2013, University of Trento
 * Authors: Luigi Palopoli <palopoli@disi.unitn.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef GENERIC_FACTORY_HPP
#define GENERIC_FACTORY_HPP
#include <memory>
#include <tinyxml2.h>
#include <map>
using namespace tinyxml2;
namespace GenericFactory {
  
  //! Template class to create and register types 
   /*!
   * This class is used to register generic type names
   * and to associate a Builder object to each of them.
   * The Builder is the root of a hierarchy,
   * The definition of new types is made by subclassing 
   * Builder. If you want to enforce unique names the
   * class should be a singleton.
   */
  template<class Builder>
  class TypeFactory {
    map<string,Builder*> type_table;
  public:
    
    //!Constructor
    /*! Does not do anything special. It simply
     * creates the table through its default constructor.
     */
    TypeFactory() : type_table() {};
    //! This method is called to register a type
    /*! It verifies that the type name has not been registered
     *  and associates it with the builder
     * \param type_name is the type name to be registered
     * \param b is a pointer to the builder
     * \return Exception in case of failure.
     */

    void register_type(const char * type_name, Builder * b) throw (Exc);

    //! Cleans up the table of the type builder

    void clean_up_types();
    
    //! Looks up in the table to search for a type
    /*!
     * \param name name of the type
     * \return A pointer to the builder associated with the type
     *   NULL if it does not exist.
     */
    Builder * look_up_type(string name);

    //! Destructor
    /*! Essentially calls clean_up_types
     */

    virtual ~TypeFactory();
  };
  

  //! Functor entity builder
  /*! This template class is to generate generic builders
   * for functor entities. It si essentially an interface class.
   */
    
  template<class FunctorEntity, typename FunctorParameters>
  class FunctorEntityBuilder {
  public:
    //! Create an instance od the functor type
    /*!
     * \param p functor type parameters, which could be themselves organised in a hiearchy
     * \return an auto_ptr to the created instance (meaning that the ownership belongs to the caller)
     */

    virtual auto_ptr<FunctorEntity> create_instance(FunctorParameters * p) throw (Exc) = 0;

    //! Parses XML segment
    /*!
     * Parses the XML segment containing the parameters used by the functor
     * \param el the XML entry related to the functor
     * \return The Functor parameters.
     */

    virtual FunctorParameters * parse_parameters(XMLElement * el) throw (Exc) = 0;
  };
  
  //! Functor entity factory
  /*! Creates a instances of functors. In our terminology a functor is a memory--less object
   * whose only purpose to offer access to user--defined functions (e.g.,
   * quality of service, or distributions. The ownership of the functor is unique. So the
   * caller receives an auto_ptr to facilitate the automated destruction of the functor
   */
  template<class FunctorEntity, typename FunctorParameters>
  class FunctorEntityFactory: public TypeFactory< FunctorEntityBuilder<FunctorEntity, FunctorParameters> > {
  public:
    //!Creates an instance of a functor with specified type_name and parameters
    /*!
     *\param type_name is the type name of the functor
     *\param p is a pointer to the functor parameters
     *\return an auto_ptr to the created functor
     */
    auto_ptr<FunctorEntity> create_instance(const char * type_name,
					    FunctorParameters * p) throw(Exc);
    //! Extracts the parameters of a functor from an XML entry
    /*! This is made in a type specific way calling the method
     * of a builder and taking advantage of a hiearchy of builders.
     * \param type_name is the type name of the functor
     * \param p is a pointer to the XML entry.
     */
    FunctorParameters * parse_parameters(const char * type_name,
				       XMLElement * p) throw (Exc);
    //! Destructor
    /*! It implicitly calls the desructor of type factors that cleans up
     * the only data structure contained in the factory.
     */
    virtual ~FunctorEntityFactory() {};
  };
  
  //! Builder template class used for named entities
  /*! it is essentially an interface class, but the signature
   * of the methods are different from the builder of a
   * functor
   */
  template<class NamedEntity, typename NamedEntityParameters>
  class NamedEntityBuilder {
  public:
    //! Method to create an instance of a named entity.
    /* Contrary to the functors, named entities have a name
     * and are stored in an internal data structure. 
     * \param type_name name of the type of the entity to be created
     * \param entity_name unique name associated with the entity.
     * \return Pointer to the named entity. Exception raised if
     *   the type does not exist. 
     */
    
    virtual NamedEntity * create_instance(const char * entity_name,
					  NamedEntityParameters * p) throw (Exc) = 0;
    
    //! Method to parse the parameters of a named entity
    /*! Using subclassing we can define type specific parsing
     * strategies (e.g., to look for type specific parameters)
     * \param entity_name name of the entity
     * \param el pointe to the XML entry data structure
     * \return the parameters of the name entity
    */
    virtual NamedEntityParameters * parse_parameters(const char * entity_name,
						 XMLElement * el) throw (Exc) = 0;
  };
  //! Factory class template to create name entities.
  /*! Creates instances of named entities. Such entities are
   * stored in a table that associates them with a unique name.
   * Multiple entries are not allowed (an exception is raised).
   */
  template<class NamedEntity, typename NamedEntityParameters>
  class NamedEntityFactory: public TypeFactory< NamedEntityBuilder<NamedEntity, NamedEntityParameters> > {
  protected:
    map<string,NamedEntity*> name_dictionary;
  public:
    //! Creates an instance of a named entity
    /*! It checks if the type does not exist, and if an entity 
     * with the same type has already been created (raising
     * an exception in this case). If the checks are passed 
     * the entity is created and stored in a table with its name.
     * \param type_name entity type
     * \param entity_name name of the entity
     * \param Pameters associated with the entity
     * \return pointer to the created entity
     */
    NamedEntity * create_instance(const char * type_name,
				  const char * entiy_name,
				  NamedEntityParameters * p);
    //!Parses an XML entity to retrieve the parameters
    /*! it extracts the parameters related to an named entity
     * This is done leveraging the builder methods for parsing
     * which is overriden to allow for type specific parameters.
     * \param type_name name of the type of the named entity
     * \param entity_name name of the entity
     * \param p pointer to the XML entry related to the named entity
     * \return pointer to the created entity (exception in case of wrong
     *    syntax.
     */
    NamedEntityParameters * parse_parameters(const char * type_name,
					     const char * entity_name,
					     XMLElement * p) throw (Exc);
    //! Retrieve an entity from its name
    /*! 
     * \param name name of the entity 
     * \return pointer to the entity if found (null otherwise)
     */
    NamedEntity * get_from_name(const char * name);
    
    //! Remove an entity from the tables
    /*! 
     * \param name name of the entity 
     * \return True if removed, False if entry does not exist
     */
    bool remove_from_name(const char * name);

    //! Cleans up the table of the names
    void clean_up();
    
    //! Destructor
    /*! calls clean up and the clean-up method of the
     * TypeFactory class
     */
    
    virtual ~NamedEntityFactory();
  };

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // IMPLEMENTATION OF THE METHODS OF TypeFactory
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   
  template<class Builder>
  void TypeFactory<Builder>::register_type(const char * type_name, Builder  * b) throw (Exc) {
    pair<string, Builder*> p(string(type_name), b);
    if (!type_table.insert(p).second)
      EXC_PRINT_2("Cannot create type", type_name);
    return;
  };

  template<class Builder>
  void TypeFactory<Builder>::clean_up_types() {
    typename map<string,Builder*>::iterator it1=type_table.begin();
       
    for (it1=type_table.begin();it1!=type_table.end();it1++) {
      Builder * q = (*it1).second;
      delete q;
    };
    type_table.erase(type_table.begin(),type_table.end());  
  };
  
  template<class Builder>
  Builder * TypeFactory<Builder>::look_up_type(string type_name) {
    typename map<string,Builder*>::iterator it;
    if ((it=type_table.find(string(type_name)))== type_table.end()) 
      return 0;
    else 
      return (*it).second;
  };

  template<class Builder>
  TypeFactory<Builder>::~TypeFactory() {
    clean_up_types();
  };
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // IMPLEMENTATION OF THE METHODS OF FunctorEntityFactory
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  template<class FunctorEntity, typename FunctorParameters>
  auto_ptr<FunctorEntity> FunctorEntityFactory<FunctorEntity,FunctorParameters>::create_instance(const char * type_name,
												 FunctorParameters * p) throw (Exc) {
    FunctorEntityBuilder<FunctorEntity, FunctorParameters> * ptr = TypeFactory< FunctorEntityBuilder<FunctorEntity, FunctorParameters> >::look_up_type(string(type_name));
    if (!ptr) 
      EXC_PRINT_2("Type undefined", type_name);
    
    return ptr->create_instance(p);
  };
  
  template<class FunctorEntity, typename FunctorParameters>
  FunctorParameters * FunctorEntityFactory<FunctorEntity,FunctorParameters>::parse_parameters(const char * type_name,
								 XMLElement * p) throw(Exc) {
    FunctorEntityBuilder<FunctorEntity, FunctorParameters> * ptr = TypeFactory< FunctorEntityBuilder<FunctorEntity, FunctorParameters> >::look_up_type(string(type_name));
    if (!ptr) 
      EXC_PRINT_2("Type undefined", type_name);
    return ptr->parse_parameters(p);
  };
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // IMPLEMENTATION OF THE METHODS OF FunctorEntityFactory
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  template<class NamedEntity, typename NamedEntityParameters>
  NamedEntity * NamedEntityFactory<NamedEntity,NamedEntityParameters>::create_instance(const char * type_name,
						    const char * entity_name,
						    NamedEntityParameters * p) {
    if (name_dictionary.find(string(entity_name))!= name_dictionary.end())
      EXC_PRINT_2("Duplicate name for named entity ",entity_name);
    NamedEntityBuilder<NamedEntity, NamedEntityParameters> * ptr =  TypeFactory< NamedEntityBuilder<NamedEntity, NamedEntityParameters> >::look_up_type(type_name);
    if(!ptr)
      EXC_PRINT_2("Type not registered with name ", type_name);
    NamedEntity * td = ptr->create_instance(entity_name, p);
    pair<string, NamedEntity* > b(string(entity_name), td);
    
    name_dictionary.insert(b);
    return td;
  }; 

  template<class NamedEntity, typename NamedEntityParameters>
  NamedEntityParameters * NamedEntityFactory<NamedEntity,NamedEntityParameters>::parse_parameters(const char * type_name,
								const char * entity_name,
								XMLElement * p) throw (Exc) {
    NamedEntityBuilder<NamedEntity, NamedEntityParameters> * ptr =  TypeFactory< NamedEntityBuilder<NamedEntity, NamedEntityParameters> >::look_up_type(type_name);
    if(!ptr)
      EXC_PRINT_2("Type not registered with name ", type_name);
    return ptr->parse_parameters(entity_name, p);
  };
  
  template<class NamedEntity, typename NamedEntityParameters>
  NamedEntity * NamedEntityFactory<NamedEntity,NamedEntityParameters>::get_from_name(const char *name) {
    typename map<string,NamedEntity*>::iterator it;
     it = name_dictionary.find(string(name));
     if (it != name_dictionary.end())
       return (*it).second;
     else
       return 0;
  };
  
  template<class NamedEntity, typename NamedEntityParameters>
  bool NamedEntityFactory<NamedEntity,NamedEntityParameters>::remove_from_name(const char *name) {
    typename map<string,NamedEntity*>::iterator it;
    it = name_dictionary.find(string(name));
    
    if (it != name_dictionary.end()) {
      name_dictionary.erase(string(name));
      return true;
    }
    else
      return false;
  };
  
  template<class NamedEntity, typename NamedEntityParameters>
  void NamedEntityFactory<NamedEntity,NamedEntityParameters>::clean_up() {
    typename map<string,NamedEntity*>::iterator it; 
    for (it=name_dictionary.begin();it!=name_dictionary.end();it++) {
      NamedEntity * p = (*it).second;
      delete p;
    };
    name_dictionary.erase(name_dictionary.begin(), name_dictionary.end());
  };
  
  template<class NamedEntity, typename NamedEntityParameters>
  NamedEntityFactory<NamedEntity,NamedEntityParameters>::~NamedEntityFactory() {
    clean_up();
  };
};



#endif
