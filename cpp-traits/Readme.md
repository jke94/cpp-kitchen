# cpp-traits

# Traits in C++

## 📌 What are traits?
**Traits** are a **design pattern using templates**, not a new language feature.  
They are used to **inject behavior or type information** at compile time, without tightly coupling classes.

Instead of your template knowing how to handle a concrete type, it delegates the logic to an external *trait*.

---

## 📌 Why use them?
- **Avoid heavy includes** → the template does not need to include external classes, only the trait.  
- **Specialize behavior per type** without changing the template.  
- **Reusability** → you can define multiple traits for different types and use them with the same template.  
- **Separation of concerns** → the template handles the generic flow, the trait handles the specific logic.

---

## 📌 Typical structure
```cpp
// 1. Generic template using a trait
template <typename T, typename Traits>
class MyTemplate {
public:
    void execute(const T& value) {
        Traits::doSomething(value);  // delegate action to the trait
    }
};

// 2. A trait with type-specific logic
struct IntTrait {
    static void doSomething(const int& x) {
        std::cout << "Processing int: " << x << "\n";
    }
};

struct StringTrait {
    static void doSomething(const std::string& s) {
        std::cout << "Processing string: " << s << "\n";
    }
};

// 3. Usage
MyTemplate<int, IntTrait> obj1;
obj1.execute(42);

MyTemplate<std::string, StringTrait> obj2;
obj2.execute("hello");

```

## ⚙️ How to build code example

1. From folder `cpp-traits` run the following script that it will compile the source code using `g++`:

```
. ./build.sh
```
2. Run the generated executable:

```
./main
```
