#include <bits/stdc++.h>
using namespace std;

class Product {
public:
    string name;
    double price;
    int quantity;
    double weight;
    bool shippable;
    time_t expiry;

    Product(string n, double p, int q, bool ship, double w = 0.0, time_t exp = 0)
        : name(n), price(p), quantity(q), shippable(ship), weight(w), expiry(exp) {}

    bool isExpired() {
        return expiry != 0 && time(0) > expiry;
    }

    bool isShippable() {
        return shippable;
    }

    void reduceQuantity(int q) {
        quantity -= q;
    }

    double getTotalWeight(int qty) {
        return weight * qty;
    }
};

class CartItem {
public:
    Product* product;
    int quantity;

    CartItem(Product* p, int q) : product(p), quantity(q) {}
};

class Cart {
public:
    vector<CartItem> items;

    void add(Product* p, int q) {
        if (q <= p->quantity) {
            items.emplace_back(p, q);
        } else {
            cout << "Error: Not enough stock for " << p->name << endl;
        }
    }

    bool isEmpty() {
        return items.empty();
    }
};

class Customer {
public:
    string name;
    double balance;

    Customer(string n, double b) : name(n), balance(b) {}

    void deduct(double amount) {
        balance -= amount;
    }
};

void checkout(Customer& customer, Cart& cart) {
    if (cart.isEmpty()) {
        cout << "Error: Cart is empty.\n";
        return;
    }

    double subtotal = 0, shipping = 0, totalWeight = 0;

    for (CartItem& item : cart.items) {
        Product* p = item.product;
        int q = item.quantity;

        if (p->isExpired()) {
            cout << "Error: Product " << p->name << " is expired.\n";
            return;
        }

        if (q > p->quantity) {
            cout << "Error: Not enough stock for " << p->name << endl;
            return;
        }

        subtotal += p->price * q;

        if (p->isShippable()) {
            shipping += 15 * q;
            totalWeight += p->getTotalWeight(q);
        }
    }

    double total = subtotal + shipping;

    if (customer.balance < total) {
        cout << "Error: Insufficient balance.\n";
        return;
    }

    cout << "** Shipment notice **\n";
    for (CartItem& item : cart.items) {
        if (item.product->isShippable()) {
            for (int i = 0; i < item.quantity; ++i) {
                cout << "Shipping: " << item.product->name << " "
                     << fixed << setprecision(1) << item.product->weight << "kg" << endl;
            }
        }
    }
    if (totalWeight > 0) {
        cout << "Total package weight: " << fixed << setprecision(1) << totalWeight << "kg\n";
    }

    for (CartItem& item : cart.items) {
        item.product->reduceQuantity(item.quantity);
    }
    customer.deduct(total);

    cout << "** Checkout receipt **\n";
    for (CartItem& item : cart.items) {
        cout << item.quantity << "x " << item.product->name << " "
             << item.product->price * item.quantity << endl;
    }
    cout << "----------------------\n";
    cout << "Subtotal " << subtotal << endl;
    cout << "Shipping " << shipping << endl;
    cout << "Amount " << total << endl;
    cout << "Remaining Balance: " << customer.balance << endl;
}

int main() {
    time_t future = time(0) + 7 * 24 * 60 * 60;

    Product cheese("Cheese", 100.0, 5, true, 0.2, future);
    Product biscuits("Biscuits", 150.0, 3, false, 0.7, future);
    Product tv("TV", 300.0, 2, true, 0.9);
    Product scratchCard("ScratchCard", 50.0, 10, false);

    Customer customer("Omar", 1000.0);
    Cart cart;

    cart.add(&cheese, 2);
    cart.add(&biscuits, 1);
    cart.add(&tv, 1);
    cart.add(&scratchCard, 1);

    checkout(customer, cart);

    return 0;
}
