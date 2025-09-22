#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Product;
class Electronics;
class Discountable;
class ShoppingCart;
class Order;

class Discountable // ChatGPT
{
private:
public:
    virtual double applyDiscount(double rate) = 0;
    virtual ~Discountable() {}
};

class Product : public Discountable
{
protected:
    int id;
    string name;
    double price;
    int sock;

public:
    Product(int i, string n, double p, int s) : id(i), name(n), price(p), sock(s) {}
    int getId()
    {
        return id;
    }
    string getName()
    {
        return name;
    }
    double getPrice()
    {
        return price;
    }
    int getSock()
    {
        return sock;
    }
    virtual void displayInfo()
    {
        cout << "|Id: " << id << endl;
        cout << "|Name: " << name << endl;
        cout << "|Price: " << price << "$" << endl;
        cout << "|Sock: " << sock << endl;
    }
    virtual void updateSock(int quantity)
    {
        if (quantity <= sock)
        {
            sock -= quantity;
        }
        else
        {
            cout << "Not enough";
        }
    }

    // Fix
    bool operator==(const Product &other) const
    {
        return this->id == other.id; // so sánh theo id
    }
    double applyDiscount(double rate) override {
        return price * (1 - rate);
    }
};

class Electronics : public Product
{
private:
    string brand;
    int warranty;

public:
    Electronics(int i, string n, double p, int s, string b, int w) : Product(i, n, p, s), brand(b), warranty(w) {}
    void displayInfo() override
    {
        cout << "Electronic device infomation" << endl;
        cout << "|Id: " << id << endl;
        cout << "|Name: " << name << endl;
        cout << "|Brand: " << brand << endl;
        cout << "|Price: " << price << "$" << endl;
        cout << "|Sock: " << sock << endl;
        cout << "|Warranty: " << warranty << " years" << endl;
    }
    void updateSock(int quantity) override
    {
        Product::updateSock(quantity);
    }
    double applyDiscount(double rate) override {
        // Ví dụ: Electronics chỉ cho giảm tối đa 50%
        if (rate > 0.5) rate = 0.5;
        return price * (1 - rate);
    }
};

template <typename T>
class InventoryList
{
private:
    vector<T> items;

public:
    void addItem(const T &item)
    {
        items.push_back(item);
    }
    void removeItem(const T &item)
    {
        for (auto it = items.begin(); it != items.end(); it++)
        {
            if (*it == item)
            {
                items.erase(it);
                return;
            }
        }
    }

    vector<T> getItems() const { return items; } // Chatgpt sua
};

class ShoppingCart
{
private:
    InventoryList<Product *> cart;
    double total;

public:
    ShoppingCart() : total(0) {}
    double calculate()
    {
        total = 0;
        for (auto &p : cart.getItems())
        {
            total += p->getPrice();
        }
        return total;
    }
    void displayTotalPrices()
    {
        cout << "|Price: " << calculate() << "$" << endl;
    }

    // ChatGPT
    //  Operator overload += to add product
    ShoppingCart &operator+=(Product *p)
    {
        if (p->getSock() <= 0)
        {
            cout << "Cannot add product. Out of stock!" << endl;
        }
        else
        {
            cart.addItem(p);
            p->updateSock(1); // decrease stock
            cout << "Product added to cart." << endl;
        }
        return *this;
    }

    // Operator overload -= to remove product
    ShoppingCart &operator-=(Product *p)
    {
        cart.removeItem(p);
        return *this;
    }
    vector<Product *> getProducts()
    {
        return cart.getItems();
    }
};

class Order
{
private:
    int orderID;
    vector<Product *> itemorder;
    double total;

public:
    Order(int id) : orderID(id), total(0) {}
    void addProduct(Product *p)
    {
        itemorder.push_back(p);
        total += p->getPrice();
    }

    void displayOrder()
    {
        cout << "|Your order " << orderID << ": " << endl;
        for (auto &p : itemorder)
        {
            p->displayInfo();
        }
        cout << "|Price total: " << total << endl;
    }
};

int main()
{
    cout << "===== TEST CASES =====" << endl;

    // 1. Creating objects
    Product p1(1, "Book", 10.0, 5);
    Product p2(2, "Pen", 2.5, 10);
    Electronics e1(3, "Laptop", 1200.0, 3, "Dell", 24);
    Electronics e2(4, "Smartphone", 800.0, 2, "Samsung", 12);

    cout << "\n--- Product Info ---" << endl;
    p1.displayInfo();
    p2.displayInfo();

    cout << "\n--- Electronics Info (Inheritance + Overridden displayInfo) ---" << endl;
    e1.displayInfo();
    e2.displayInfo();

    // 2. Operator overloading: ==
    cout << "\n--- Operator == ---" << endl;
    if (p1 == p2)
        cout << "Book and Pen are the same product." << endl;
    else
        cout << "Book and Pen are different products." << endl;

    Product p3(1, "Notebook", 15.0, 3);
    if (p1 == p3)
        cout << "Book and Notebook are the same product (same ID)." << endl;

    // 3. Interface: applyDiscount polymorphically
    cout << "\n--- Apply Discount via Interface ---" << endl;
    Discountable *d1 = &p1;
    Discountable *d2 = &e1;
    cout << "Book price after 20% discount: " << d1->applyDiscount(0.2) << endl;
    cout << "Laptop price after 10% discount: " << d2->applyDiscount(0.1) << endl;

    // 4. ShoppingCart with operator overloading
    ShoppingCart cart;
    cout << "\n--- Add Products to Cart (+=) ---" << endl;
    cart += &p1; // add Book
    cart += &p2; // add Pen
    cart += &e1; // add Laptop
    cart.displayTotalPrices();

    cout << "\n--- Remove Product from Cart (-=) ---" << endl;
    cart -= &p2; // remove Pen
    cart.displayTotalPrices();

    // 5. Operations: updateSock, handle errors
    cout << "\n--- Update Stock & Handle Out-of-Stock ---" << endl;
    p1.updateSock(2); // reduce stock
    cout << "Book stock after selling 2: " << p1.getSock() << endl;
    p1.updateSock(10); // should print error (not enough stock)

    // 6. Order creation
    cout << "\n--- Create Order from Cart ---" << endl;
    Order order1(101);
    for (auto &prod : cart.getProducts())
    {
        order1.addProduct(prod);
    }
    order1.displayOrder();

    // 7. Template Class InventoryList
    cout << "\n--- InventoryList<Product*> ---" << endl;
    InventoryList<Product *> inventory;
    inventory.addItem(&p1);
    inventory.addItem(&e2);
    for (auto &prod : inventory.getItems())
    {
        prod->displayInfo();
    }

    cout << "\n--- InventoryList<int> (Template test with different type) ---" << endl;
    InventoryList<int> numbers;
    numbers.addItem(10);
    numbers.addItem(20);
    numbers.addItem(30);
    for (auto &n : numbers.getItems())
    {
        cout << "Number: " << n << endl;
    }

    cout << "\n===== END OF TESTS =====" << endl;
    return 0;
}