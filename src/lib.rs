#![feature(box_syntax, box_patterns)]
#[derive(PartialEq, PartialOrd, Debug)]
enum Term
{
    TString(String),
    TBool(bool),
    TCon(Box<Term>, Box<Term>, Box<Term>),
    TPrintLn(Box<Term>),
    TUnit,
    TSeq(Box<Term>, Box<Term>)
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
        TSeq(box l, box r) => { eval(l); eval(r) }
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
