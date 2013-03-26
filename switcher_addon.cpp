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
#include <v8.h>
#include <uv.h>
#include <switcher/quiddity-manager.h>

static std::vector<switcher::QuiddityManager::ptr> switcher_container;
static v8::Persistent<v8::Function> user_log_cb; //must be disposed


struct async_req {
  uv_work_t req;
  std::string msg;
};


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
  user_log_cb.Dispose ();
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

// log callback
v8::Handle<v8::Value> RegisterLogCallback(const v8::Arguments& args) {
  
  v8::HandleScope scope;
  user_log_cb = v8::Persistent<v8::Function>::New(v8::Local<v8::Function>::Cast(args[0]));
    const unsigned argc = 1;
    v8::Local<v8::Value> argv[argc] = { v8::Local<v8::Value>::New(v8::String::New("hello world")) };
    user_log_cb->Call(v8::Context::GetCurrent()->Global(), argc, argv);
  
  return scope.Close(v8::Undefined());
}


void DoNothingAsync (uv_work_t *r) {
}

void NotifyLog (uv_work_t *r) {
  v8::HandleScope scope;
  async_req *req = reinterpret_cast<async_req *>(r->data);
  v8::TryCatch try_catch;
  v8::Local<v8::Value> argv[] = { v8::Local<v8::Value>::New(v8::String::New(req->msg.c_str ())) };
  user_log_cb->Call(user_log_cb, 1, argv);
  delete req;
  if (try_catch.HasCaught()) {
    node::FatalException(try_catch);
  }
}

//call client log callback
static void 
logger_cb (std::string quiddity_name, std::string property_name, std::string value, void *user_data)
{
    async_req *req = new async_req;
    req->req.data = req;
    req->msg = value;
    uv_queue_work (uv_default_loop(),
		   &req->req,
		   DoNothingAsync,
		   (uv_after_work_cb)NotifyLog);
}



// ------------ node init functions -------------------------------
void Init(v8::Handle<v8::Object> target) {

  switcher::QuiddityManager::ptr switcher_manager 
    = switcher::QuiddityManager::make_manager ("nodeserver");  
  
   switcher_manager->create ("logger", "internal_logger");
   switcher_manager->invoke_va ("internal_logger", "install_log_handler", "shmdata", NULL);
   switcher_manager->invoke_va ("internal_logger", "install_log_handler", "GStreamer", NULL);
   switcher_manager->invoke_va ("internal_logger", "install_log_handler", "Glib", NULL);
   switcher_manager->invoke_va ("internal_logger", "install_log_handler", "Glib-GObject", NULL);
   switcher_manager->set_property ("internal_logger", "mute", "false");
   switcher_manager->set_property ("internal_logger", "debug", "true");
   switcher_manager->set_property ("internal_logger", "verbose", "true");
   switcher_manager->make_subscriber ("log_sub", logger_cb, NULL);
   switcher_manager->subscribe_property ("log_sub","internal_logger","last-line");
  
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
  
  //calback
   target->Set(v8::String::NewSymbol("register_log_callback"),
    	      v8::FunctionTemplate::New(RegisterLogCallback)->GetFunction());
}

NODE_MODULE(switcher_addon, Init)
