#![feature(box_syntax, box_patterns)]
#[derive(PartialEq, PartialOrd, Debug, Clone)]
enum Term
{
    TString(String),
    TBool(bool),
    TCon(Box<Term>, Box<Term>, Box<Term>),
    TWhen(Box<Term>, Box<Term>),
    TUnless(Box<Term>, Box<Term>),
    TPrintLn(Box<Term>),
    TPrint(Box<Term>),
    TUnit,
    TSeq(Box<Term>, Box<Term>),
    TShow(Box<Term>),
    TAnd(Box<Term>, Box<Term>),
    TOr(Box<Term>, Box<Term>),
    TNot(Box<Term>),
    TWhile(Box<Term>, Box<Term>)
}
use Term::*;
fn eval(t : Term) -> Term
{
    match t
    {
        TString(_) => t,
        TBool(_) => t,
        TUnit => t,
        TCon(box i, box t, box e) =>
            match eval(i) {
                TBool(true) => eval(t),
                TBool(false) => eval(e),
                _ => unreachable!()
            },
        TPrintLn(box s) =>
            match eval(s) {
                TString(x) => { println!("{}", x); TUnit },
                _ => unreachable!()
            },
        TPrint(box s) =>
            match eval(s) {
                TString(x) => { print!("{}", x); TUnit },
                _ => unreachable!()
            },
        TSeq(box l, box r) => { eval(l); eval(r) },
        TShow(box x) =>
            match eval(x) {
                TString(x) => TString(x),
                TBool(true) => TString(String::from("true")),
                TBool(false) => TString(String::from("false")),
                TUnit => TString(String::from("unit")),
                _ => unreachable!()
            },
        TWhen(c, act) => eval(TCon(c, act, box TUnit)),
        TUnless(c, act) => eval(TCon(c, box TUnit, act)),
        TAnd(l, r) => eval(TCon(l, r, box TBool(false))),
        TOr(l, r) => eval(TCon(l, box TBool(true), r)),
        TNot(c) => eval(TCon(c, box TBool(false), box TBool(true))),
        TWhile(c, act) =>
            eval(TWhen(c.clone(),
                       box TSeq(act.clone(), box TWhile(c, act))))
    }
}
#[test]
fn test_conditional(){
    assert_eq!(eval(TCon(
        box TBool(true),
        box TString(String::from("Hello")),
        box TString(String::from("World")))),
               TString(String::from("Hello")))
}
