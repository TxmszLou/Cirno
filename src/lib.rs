#![feature(box_syntax, box_patterns)]
Enum Term {
    TString(String),
    TBool(bool),
    TCon(Box<Term>, Box<Term>, Box<Term>)
}
use Term::*;
fn eval(t : Term) -> Term {
    match t {
        TString(_) => t,
        TBool(_) => t,
        TCon(box i, box t, box e) =>
            match eval(i) {
                TBool(true) => eval(t),
                TBool(false) => eval(e),
                _ => unreachable!()
            }
    }
}
#[test]
fn it_works() {
}
