#ifndef _HISTORY_TRANSACTION_HPP_
#define _HISTORY_TRANSACTION_HPP_

#include <cassert>
#include <iomanip>
#include <iostream>

#include "project3.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK
//
Transaction::Transaction( std::string ticker_symbol,  unsigned int day_date,
unsigned int month_date,  unsigned year_date,
bool buy_sell_trans,  unsigned int number_shares,
double trans_amount ) {
    symbol = ticker_symbol;
    day = day_date;
    month = month_date;
    year = year_date;
    if (buy_sell_trans) {
        trans_type = "Buy";
    } else {
        trans_type = "Sell";
    }
    shares = number_shares;
    amount = trans_amount;
    trans_id = assigned_trans_id;
    assigned_trans_id++;
    p_next = nullptr;
    acb = 0;
    acb_per_share = 0;
    share_balance = 0;
    cgl = 0;

    /*
    acb = acb + amount;
    get_next()->set_acb(acb);
    if (buy_sell_trans) {
        share_balance += shares;
    } else {
        share_balance -= shares;
    }
    get_next()->set_share_balance(share_balance);
    acb_per_share = acb/share_balance;
    if (!buy_sell_trans) {
        cgl += amount - (shares * acb_per_share);
    }
    get_next()->set_cgl(cgl);
     */
}



// Destructor
// TASK
//
Transaction::~Transaction() {

}

// TASK
// Overloaded < operator.
//
bool Transaction::operator<(Transaction const &other){
    if (this->year < other.get_year()) {
        return true;
    } else if (other.get_year() < this->year) {
        return false;
    }

    if (this->month < other.get_month()) {
        return true;
    } else if (other.get_month() < this->month) {
        return false;
    }

    if (this->day < other.get_day()) {
        return true;
    } else if (other.get_day() < this->day) {
        return false;
    }
    return false;
}
// GIVEN
// Member functions to get values. 
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values. 
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " " 
    << std::setw(4) << get_symbol() << " " 
    << std::setw(4) << get_day() << " "  
    << std::setw(4) << get_month() << " " 
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) { 
    std::cout << "  Buy  "; 
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " " 
    << std::setw(10) << get_amount() << " " 
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl() 
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK
//
History::History() {
    p_head = nullptr;
}

// Destructor
// TASK
//
History::~History() {
    Transaction *p = p_head;
    while (p_head != nullptr) {
        p = p->get_next();
        delete p_head;
        p_head = p;
    }
}

// TASK
// read_transaction(...): Read the transaction history from file. 
//
void History::read_history() {
    ece150::open_file();
    while (ece150::next_trans_entry()) {
        History::insert(new Transaction(ece150::get_trans_symbol(), ece150::get_trans_day(), ece150::get_trans_month(), ece150::get_trans_year(), ece150::get_trans_type(), ece150::get_trans_shares(), ece150::get_trans_amount()));
    }
}

// insert(...): Insert transaction into linked list.
//
void History::insert(Transaction *p_new_trans) {
    if (p_head == nullptr) {
        p_head = p_new_trans;
        p_head->set_next(nullptr);
    } else {
        Transaction * p = p_head;
        while (p->get_next() != nullptr) {
            p = p->get_next();
        }
        p->set_next(p_new_trans);
    }
}


// TASK
// sort_by_date(): Sort the linked list by trade date.
//
void History::sort_by_date() {
    Transaction *p = p_head;
    int a = 0;
    while (p->get_next() != nullptr) {
        a++;
        p = p->get_next();
    }
    Transaction *hold;
    Transaction *l;
    for (int i = 0; i < a; i++) {
        p = p_head->get_next();
        l = p_head->get_next();
        if (*p_head->get_next() < *p_head) {
            hold = p_head->get_next();
            p_head->set_next(hold->get_next());
            hold->set_next(p_head);
            p_head = hold;
        }
        while (p->get_next() != nullptr) {
            p = p->get_next();
            if (p->get_next() == nullptr) {
                break;
            }
            if (*p->get_next() < *p) {
                hold = p->get_next();
                p->set_next(hold->get_next());
                hold->set_next(p);
                l->set_next(hold);
                p = hold;
            }
            l = l->get_next();
        }

    }
}


// TASK
// update_acb_cgl(): Updates the ACB and CGL values. 
//
void History::update_acb_cgl() {
    Transaction *p = p_head;
    while (p != nullptr) {
        if (p->get_trans_type()) {
            p->set_acb(p->get_acb() + p->get_amount());
            p->set_share_balance(p->get_share_balance() + p->get_shares());
        } else {
            p->set_acb(p->get_acb() - (p->get_shares() * p->get_acb_per_share()));
            p->set_share_balance(p->get_share_balance() - p->get_shares());
        }

        if (p->get_next() != nullptr) {
            p->get_next()->set_acb(p->get_acb());
            p->get_next()->set_share_balance(p->get_share_balance());
        }

        if (p->get_trans_type()) {
            p->set_acb_per_share(p->get_acb() / p->get_share_balance());
        }
        if (p->get_next() != nullptr) {
            p->get_next()->set_acb_per_share(p->get_acb_per_share());
        }


        if (!p->get_trans_type()) {
            p->set_cgl(p->get_amount() - (p->get_shares() * p->get_acb_per_share()));
        }

        p = p->get_next();
    }
}


// TASK
// compute_cgl(): )Compute the ACB, and CGL.
//
double History::compute_cgl(unsigned int year) {
    Transaction *p = p_head;
    double ret = 0;

    while (p != nullptr) {
        if (p->get_year() == year) {
            if (!p->get_trans_type()) {
                ret += p->get_amount() - (p->get_shares() * p->get_acb_per_share());
            }
        }
        p = p->get_next();
    }
    return ret;
}



// TASK
// print() Print the transaction history.
//
void History::print() {
    Transaction *p = p_head;
    while (p != nullptr) {
        p->print();
        p = p->get_next();
    }
}



// GIVEN
// get_p_head(): Full access to the linked list.
// 
Transaction *History::get_p_head() { return p_head; }


#endif

