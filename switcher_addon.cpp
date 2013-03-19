/*
 * Copyright (C) 2012-2013 Nicolas Bouillot (http://www.nicolasbouillot.net)
 *
 * This file is part of node-switcher.
 *
 * node-switcher is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * node-switcher is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with node-switcher.  If not, see <http://www.gnu.org/licenses/>.
 */

//#define BUILDING_NODE_EXTENSION
#include <node.h>

#include <switcher/quiddity-manager.h>

static std::vector<switcher::QuiddityManager::ptr> switcher_container;


// ----------- life management
v8::Handle<v8::Value> Remove(const v8::Arguments& args) {
  v8::HandleScope scope;
  if (args.Length() != 1) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong number of arguments")));
    return scope.Close(v8::Undefined());
  }
  if (!args[0]->IsString() ) {
    ThrowException(v8::Exception::TypeError(v8::String::New("switcher create: Wrong first arguments type")));
    return scope.Close(v8::Undefined());
  }
  v8::String::AsciiValue first_arg(args[0]->ToString());
  if (switcher_container[0]->remove(std::string(*first_arg)))
    return v8::Boolean::New(true);
  else
    return v8::Boolean::New(false);
}

v8::Handle<v8::Value> Create(const v8::Arguments& args) {
  v8::HandleScope scope;
  if (args.Length() != 1 && args.Length() != 2) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong number of arguments")));
    return scope.Close(v8::Undefined());
  }
  if (!args[0]->IsString() ) {
    ThrowException(v8::Exception::TypeError(v8::String::New("switcher create: Wrong first arg type")));
    return scope.Close(v8::Undefined());
  }
  v8::String::AsciiValue first_arg(args[0]->ToString());
  v8::Local<v8::String> name;
  if (args.Length() ==2)
    {
      if (!args[1]->IsString() ) {
	ThrowException(v8::Exception::TypeError(v8::String::New("switcher create: Wrong second arg type")));
	return scope.Close(v8::Undefined());
      }
      v8::String::AsciiValue second_arg(args[1]->ToString());
      name = v8::String::New(switcher_container[0]->create(std::string(*first_arg), 
							   std::string(*second_arg)).c_str());
    }
  else
    name = v8::String::New(switcher_container[0]->create(std::string(*first_arg)).c_str());

  return scope.Close(name);
}

v8::Handle<v8::Value> SwitcherClose(const v8::Arguments& args) {
  v8::HandleScope scope;
  //removing reference to manager in order to delete it
  switcher_container.clear ();
  v8::Local<v8::String> name = v8::String::New("closed");
  return scope.Close(name);
}

v8::Handle<v8::Value> GetClassesDoc(const v8::Arguments& args) {
  v8::HandleScope scope;

  v8::Handle<v8::String> res = 
    v8::String::New(switcher_container[0]->get_classes_doc().c_str());
  return scope.Close(res);
}

v8::Handle<v8::Value> GetClassDoc(const v8::Arguments& args) {
  v8::HandleScope scope;
  if (args.Length() != 1) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong number of arguments")));
    return scope.Close(v8::Undefined());
  }
  if (!args[0]->IsString()) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong arguments")));
    return scope.Close(v8::Undefined());
  }
  v8::String::AsciiValue class_name(args[0]->ToString());

  v8::Handle<v8::String> res = 
    v8::String::New(switcher_container[0]->get_class_doc(std::string(*class_name)).c_str() );
  return scope.Close(res);
}

v8::Handle<v8::Value> GetQuiddityDescription(const v8::Arguments& args) {
  v8::HandleScope scope;
  if (args.Length() != 1) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong number of arguments")));
    return scope.Close(v8::Undefined());
  }
  if (!args[0]->IsString()) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong arguments")));
    return scope.Close(v8::Undefined());
  }
  v8::String::AsciiValue quiddity_name(args[0]->ToString());

  v8::Handle<v8::String> res = 
    v8::String::New(switcher_container[0]->get_quiddity_description(std::string(*quiddity_name)).c_str() );
  return scope.Close(res);
}

v8::Handle<v8::Value> GetQuidditiesDescription(const v8::Arguments& args) {
  v8::HandleScope scope;

  v8::Handle<v8::String> res = 
    v8::String::New(switcher_container[0]->get_quiddities_description().c_str());
  return scope.Close(res);
}

//end life manager

// ----------- properties
v8::Handle<v8::Value> SetProperty(const v8::Arguments& args) {
  v8::HandleScope scope;
  if (args.Length() != 3) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong number of arguments")));
    return scope.Close(v8::Undefined());
  }
  if (!args[0]->IsString() || !args[1]->IsString() || !args[2]->IsString ()) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong arguments")));
    return scope.Close(v8::Undefined());
  }
  v8::String::AsciiValue element_name(args[0]->ToString());
  v8::String::AsciiValue property_name(args[1]->ToString());
  v8::String::AsciiValue property_val(args[2]->ToString());

  v8::Handle<v8::Boolean> res = 
    v8::Boolean::New(switcher_container[0]->set_property(std::string(*element_name), 
							 std::string(*property_name), 
							 std::string(*property_val)));
  return scope.Close(res);
}

v8::Handle<v8::Value> GetProperty(const v8::Arguments& args) {
  v8::HandleScope scope;
  if (args.Length() != 2) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong number of arguments")));
    return scope.Close(v8::Undefined());
  }
  if (!args[0]->IsString() || !args[1]->IsString()) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong arguments")));
    return scope.Close(v8::Undefined());
  }
  v8::String::AsciiValue element_name(args[0]->ToString());
  v8::String::AsciiValue property_name(args[1]->ToString());

  v8::Handle<v8::String> res = 
    v8::String::New(switcher_container[0]->get_property(std::string(*element_name), 
							std::string(*property_name)).c_str());
  return scope.Close(res);
}

v8::Handle<v8::Value> GetPropertiesDescription(const v8::Arguments& args) {
  v8::HandleScope scope;
  if (args.Length() != 1) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong number of arguments")));
    return scope.Close(v8::Undefined());
  }
  if (!args[0]->IsString()) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong arguments")));
    return scope.Close(v8::Undefined());
  }
  v8::String::AsciiValue element_name(args[0]->ToString());

  v8::Handle<v8::String> res = 
    v8::String::New(switcher_container[0]->get_properties_description(std::string(*element_name)).c_str());
  return scope.Close(res);
}

v8::Handle<v8::Value> GetPropertyDescription(const v8::Arguments& args) {
  v8::HandleScope scope;
  if (args.Length() != 2) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong number of arguments")));
    return scope.Close(v8::Undefined());
  }
  if (!args[0]->IsString() || !args[1]->IsString()) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong arguments")));
    return scope.Close(v8::Undefined());
  }
  v8::String::AsciiValue element_name(args[0]->ToString());
  v8::String::AsciiValue property_name(args[1]->ToString());

  v8::Handle<v8::String> res = 
    v8::String::New(switcher_container[0]->get_property_description(std::string(*element_name), 
								    std::string(*property_name)).c_str());
  return scope.Close(res);
}

v8::Handle<v8::Value> GetPropertiesDescriptionByClass(const v8::Arguments& args) {
  v8::HandleScope scope;
  if (args.Length() != 1) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong number of arguments")));
    return scope.Close(v8::Undefined());
  }
  if (!args[0]->IsString()) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong arguments")));
    return scope.Close(v8::Undefined());
  }
  v8::String::AsciiValue class_name(args[0]->ToString());

  v8::Handle<v8::String> res = 
    v8::String::New(switcher_container[0]->get_properties_description_by_class(std::string(*class_name)).c_str());
  return scope.Close(res);
}

v8::Handle<v8::Value> GetPropertyDescriptionByClass(const v8::Arguments& args) {
  v8::HandleScope scope;
  if (args.Length() != 2) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong number of arguments")));
    return scope.Close(v8::Undefined());
  }
  if (!args[0]->IsString() || !args[1]->IsString()) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong arguments")));
    return scope.Close(v8::Undefined());
  }
  v8::String::AsciiValue class_name(args[0]->ToString());
  v8::String::AsciiValue property_name(args[1]->ToString());

  v8::Handle<v8::String> res = 
    v8::String::New(switcher_container[0]->get_property_description_by_class(std::string(*class_name), 
									     std::string(*property_name)).c_str());
  return scope.Close(res);
}
//end properties

// ----------- methods
v8::Handle<v8::Value> Invoke(const v8::Arguments& args) {
  v8::HandleScope scope;
  if (args.Length() < 3) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong number of arguments")));
    return scope.Close(v8::Undefined());
  }
  if (!args[0]->IsString() || !args[1]->IsString() || !args[2]->IsArray ()) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong arguments")));
    return scope.Close(v8::Undefined());
  }
  v8::String::AsciiValue element_name(args[0]->ToString());
  v8::String::AsciiValue function_name(args[1]->ToString());
  v8::Local<v8::Object> obj_arguments = args[2]->ToObject();
  v8::Local<v8::Array> arguments = obj_arguments->GetPropertyNames();

  std::vector<std::string> vector_arg;
  for(unsigned int i = 0; i < arguments->Length(); i++) {
    v8::String::AsciiValue val(obj_arguments->Get(i)->ToString());
    vector_arg.push_back(std::string(*val));
    }
  v8::Handle<v8::Boolean> res = 
    v8::Boolean::New(switcher_container[0]->invoke(std::string(*element_name),
						   std::string(*function_name),
						   vector_arg));
  return scope.Close(res);
}

v8::Handle<v8::Value> GetMethodsDescription(const v8::Arguments& args) {
  v8::HandleScope scope;
  if (args.Length() != 1) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong number of arguments")));
    return scope.Close(v8::Undefined());
  }
  if (!args[0]->IsString()) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong arguments")));
    return scope.Close(v8::Undefined());
  }
  v8::String::AsciiValue element_name(args[0]->ToString());

  v8::Handle<v8::String> res = 
    v8::String::New(switcher_container[0]->get_methods_description(std::string(*element_name)).c_str());
  return scope.Close(res);
}

v8::Handle<v8::Value> GetMethodDescription(const v8::Arguments& args) {
  v8::HandleScope scope;
  if (args.Length() != 2) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong number of arguments")));
    return scope.Close(v8::Undefined());
  }
  if (!args[0]->IsString() || !args[1]->IsString()) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong arguments")));
    return scope.Close(v8::Undefined());
  }
  v8::String::AsciiValue element_name(args[0]->ToString());
  v8::String::AsciiValue method_name(args[1]->ToString());

  v8::Handle<v8::String> res = 
    v8::String::New(switcher_container[0]->get_method_description(std::string(*element_name), 
								  std::string(*method_name)).c_str());
  return scope.Close(res);
}

v8::Handle<v8::Value> GetMethodsDescriptionByClass(const v8::Arguments& args) {
  v8::HandleScope scope;
  if (args.Length() != 1) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong number of arguments")));
    return scope.Close(v8::Undefined());
  }
  if (!args[0]->IsString()) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong arguments")));
    return scope.Close(v8::Undefined());
  }
  v8::String::AsciiValue class_name(args[0]->ToString());

  v8::Handle<v8::String> res = 
    v8::String::New(switcher_container[0]->get_methods_description_by_class(std::string(*class_name)).c_str());
  return scope.Close(res);
}

v8::Handle<v8::Value> GetMethodDescriptionByClass(const v8::Arguments& args) {
  v8::HandleScope scope;
  if (args.Length() != 2) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong number of arguments")));
    return scope.Close(v8::Undefined());
  }
  if (!args[0]->IsString() || !args[1]->IsString()) {
    ThrowException(v8::Exception::TypeError(v8::String::New("Wrong arguments")));
    return scope.Close(v8::Undefined());
  }
  v8::String::AsciiValue class_name(args[0]->ToString());
  v8::String::AsciiValue method_name(args[1]->ToString());

  v8::Handle<v8::String> res = 
    v8::String::New(switcher_container[0]->get_method_description_by_class(std::string(*class_name), 
									   std::string(*method_name)).c_str());
  return scope.Close(res);
}




// ------------ node init functions -------------------------------
void Init(v8::Handle<v8::Object> target) {

  switcher::QuiddityManager::ptr switcher_manager 
    = switcher::QuiddityManager::make_manager ("nodeserver");  
  switcher_manager->create ("runtime");
  switcher_container.push_back (switcher_manager); // keep reference only in the container
  //setting auto_invoke for attaching to gst pipeline "pipeline0"
  std::vector<std::string> arg;
  arg.push_back ("pipeline0");
  switcher_manager->auto_invoke ("set_runtime",arg);

  //life manager
  target->Set(v8::String::NewSymbol("create"),
	      v8::FunctionTemplate::New(Create)->GetFunction());  
  target->Set(v8::String::NewSymbol("remove"),
	      v8::FunctionTemplate::New(Remove)->GetFunction());  
  target->Set(v8::String::NewSymbol("close"),
	      v8::FunctionTemplate::New(SwitcherClose)->GetFunction());  
  target->Set(v8::String::NewSymbol("get_classes_doc"),
	      v8::FunctionTemplate::New(GetClassesDoc)->GetFunction());  
  target->Set(v8::String::NewSymbol("get_class_doc"),
	      v8::FunctionTemplate::New(GetClassDoc)->GetFunction());  
  target->Set(v8::String::NewSymbol("get_quiddity_description"),
   	      v8::FunctionTemplate::New(GetQuiddityDescription)->GetFunction());  
  target->Set(v8::String::NewSymbol("get_quiddities_description"),
	      v8::FunctionTemplate::New(GetQuidditiesDescription)->GetFunction());  

  //properties
  target->Set(v8::String::NewSymbol("get_properties_description"),
	      v8::FunctionTemplate::New(GetPropertiesDescription)->GetFunction());  
  target->Set(v8::String::NewSymbol("get_property_description"),
	      v8::FunctionTemplate::New(GetPropertyDescription)->GetFunction());  
  target->Set(v8::String::NewSymbol("get_properties_description_by_class"),
	      v8::FunctionTemplate::New(GetPropertiesDescriptionByClass)->GetFunction());  
  target->Set(v8::String::NewSymbol("get_property_description_by_class"),
	      v8::FunctionTemplate::New(GetPropertyDescriptionByClass)->GetFunction());  
  target->Set(v8::String::NewSymbol("set_property_value"),
	      v8::FunctionTemplate::New(SetProperty)->GetFunction());  
  target->Set(v8::String::NewSymbol("get_property_value"),
	      v8::FunctionTemplate::New(GetProperty)->GetFunction());  
  //methods
  target->Set(v8::String::NewSymbol("get_methods_description"),
	      v8::FunctionTemplate::New(GetMethodsDescription)->GetFunction());  
  target->Set(v8::String::NewSymbol("get_method_description"),
	      v8::FunctionTemplate::New(GetMethodDescription)->GetFunction());  
  target->Set(v8::String::NewSymbol("get_methods_description_by_class"),
	      v8::FunctionTemplate::New(GetMethodsDescriptionByClass)->GetFunction());  
  target->Set(v8::String::NewSymbol("get_method_description_by_class"),
	      v8::FunctionTemplate::New(GetMethodDescriptionByClass)->GetFunction());  
  target->Set(v8::String::NewSymbol("invoke"),
	      v8::FunctionTemplate::New(Invoke)->GetFunction());  
  
}

NODE_MODULE(switcher_addon, Init)
