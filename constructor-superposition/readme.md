# Using Custom class object 

## Map<String, Harish>

```c++
map<string, Harish> a;
a["z"]=Harish(79);
```

```
Parameterized constructor...
Default Constructor...
```

**It appears that two objects are created**:
- First the parameterized constructor is called to create a temp object of the rvalue.
- An implicit move constructor is called which somehow is using the default constructor? 🤷‍♂️
    - This is because when we define a custom move constructor the compilation fails saying we also need to override the default `=` (copy assignment) operator.

**Note: Apparently the move constructor is supposed to delete the created temp object. Something to do with `&&`?**

## Vector<Harish>

### Code

```cpp
vector<Harish> y;
y.push_back(Harish(63));
```

### Output

**If copy constructor is available**

```
Parameterized constructor...
Copy constructor...
```

**If a move constructor is also available**

```
Parameterized constructor...
Move constructor...
```

### Inference

**It appears that two objects are created**:

- First the parameterized constructor is used to create a temp object of the rvalue.
- Then a copy constructor is used to create the object being pushed in the vector.
    - What happens to the temp object? 
        - It does not get delted (I think!).
- If a move consturctor is also available then it is used to create the object being pushed in the vector.
    - This should somehow ensure that there is only one copy of the object.

**Note:** 

- Only Anonymous construction calls move construtor. Otherwise copy constructor is called.
- [Move Constructor](https://en.cppreference.com/w/cpp/language/move_constructor)
# Using reference to custom class object (Not Allowed)
 
If we create map and vector of references then you can't do anonymous initialization of map and vector variables.

```cpp
map<string, Harish&> a;
a["z"]=Harish(79);
```
```
error: reference to type 'Harish' requires an initializer
```

```cpp
vector<Harish&> y;
y.push_back(Harish(63));
```
```
/Library/Developer/CommandLineTools/usr/bin/../include/c++/v1/memory:851:43: error: type 'int' cannot be used prior to '::' because it has no members
    typedef _LIBCPP_NODEBUG_TYPE typename _Tp::template rebind<_Up> type;
```

Why c++ errors are so complex and unhelpful. 🤦‍♂️

**Kurzgesagt**

The component type of containers like vectors must be assignable. References are not assignable (you can only initialize them once when they are declared, and you cannot make them reference something else later). Other non-assignable types are also not allowed as components of containers, e.g. vector<const int> is not allowed.

# Using a pointer to custom class Object

### Anonymous Initialization not allowed.
```cpp
map<string, Harish*> a;
a["z"]=&Harish(79);

vector<Harish*> y;
y.push_back(&Harish(63));
```

```
map_and_vector_of_pointer_to_object.cpp:48:12: error: taking the address of a temporary object of type 'Harish' [-Waddress-of-temporary]
    a["z"]=&Harish(79);
           ^~~~~~~~~~~
map_and_vector_of_pointer_to_object.cpp:52:17: error: taking the address of a temporary object of type 'Harish' [-Waddress-of-temporary]
    y.push_back(&Harish(63));
                ^~~~~~~~~~~
2 errors generated.
```
### Intialize with an object

```cpp
Harish h(79);
map<string, Harish *> a;
a["z"] = &h;

vector<Harish *> y;
y.push_back(&h);
```

```
Parameterized constructor...
```

Only one object of class Harish is created.