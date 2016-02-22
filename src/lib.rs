#![feature(box_syntax, box_patterns)]
#[derive(PartialEq, PartialOrd, Debug, Clone)]
pub enum Term
{
    TString(String),
    TBool(bool),
    TConcat(Box<Term>, Box<Term>),
    TCon(Box<Term>, Box<Term>, Box<Term>),
    TWhen(Box<Term>, Box<Term>),
    TUnless(Box<Term>, Box<Term>),
    TPrintLn(Box<Term>),
    TUnit,
    TSeq(Box<Term>, Box<Term>),
    TShow(Box<Term>),
    TAnd(Box<Term>, Box<Term>),
    TOr(Box<Term>, Box<Term>),
    TNot(Box<Term>),
    TWhile(Box<Term>, Box<Term>),
    TS,
    TK,
    TI,
    TApp(Box<Term>, Box<Term>)
}
use Term::*;
pub trait VM {
    fn println(&mut self, &String);
}
pub fn eval<ENV : VM>(env : &mut ENV, tr : & Term) -> Term
{
    match tr
    {
        &TString(_) => tr.clone(),
        &TBool(_) => tr.clone(),
        &TUnit => tr.clone(),
        &TS => tr.clone(),
        &TK => tr.clone(),
        &TI => tr.clone(),
        &TApp(box ref l, box ref r) =>
            match (eval(env, &l), eval(env, &r)) {
                (TApp(box TS, box TApp(x, y)), z) =>
                    eval(env,
                         &TApp(box TApp(x, box z.clone()), box TApp(y, box z))),
                (TK, x) => TApp(box TK, box x),
                (TI, x) => x,
                (TApp(box TK, box x), _) => x,
                (TS, x) => TApp(box TS, box x),
                (TApp(box TS, x), y) => TApp(box TApp(box TS, x), box y),
                _ => unreachable!()
            },
        &TCon(box ref i, box ref t, box ref e) =>
            match eval(env, &i) {
                TBool(true) => eval(env, t),
                TBool(false) => eval(env, e),
                _ => unreachable!()
            },
        &TPrintLn(box ref s) =>
            match eval(env, &s) {
                TString(x) => { env.println(&x); TUnit },
                _ => unreachable!()
            },
        &TConcat(box ref l, box ref r) =>
            match (eval(env, l), eval(env, r)) {
                (TString(ls), TString(rs)) => TString(ls + &rs),
                _ => unreachable!()
            },
        &TSeq(box ref l, box ref r) => { eval(env, l); eval(env, r) },
        &TShow(box ref x) =>
            match eval(env, x) {
                TString(x) => TString(x),
                TBool(true) => TString(String::from("true")),
                TBool(false) => TString(String::from("false")),
                TUnit => TString(String::from("unit")),
                _ => unreachable!()
            },
        &TWhen(box ref c, box ref act) =>
            eval(env, &TCon(box c.clone(), box act.clone(), box TUnit)),
        &TUnless(box ref c, box ref act) =>
            eval(env, &TCon(box c.clone(), box TUnit, box act.clone())),
        &TAnd(box ref l, box ref r) =>
            eval(env, &TCon(box l.clone(), box r.clone(), box TBool(false))),
        &TOr(box ref l, box ref r) =>
            eval(env, &TCon(box l.clone(), box TBool(true), box r.clone())),
        &TNot(box ref c) =>
            eval(env, &TCon(box c.clone(), box TBool(false), box TBool(true))),
        &TWhile(box ref c, box ref act) =>
            eval(env,
                 &TWhen(box c.clone(),
                        box TSeq(box act.clone(),
                                 box TWhile(box c.clone(), box act.clone() ))))
    }
}
use std::collections::LinkedList;
pub struct TestVM {
    output : LinkedList<String>
}
impl TestVM {
    pub fn new() -> TestVM {
        TestVM { output : LinkedList::new() }
    }
}
impl VM for TestVM {
    fn println(&mut self, x : &String) {
        self.output.push_back(x.clone())
    }
}

pub struct DefaultVM;

impl VM for DefaultVM {
    fn println(&mut self, x : &String) {
        println!("{}", x)
    }
}

#[test]
fn test_conditional(){
    assert_eq!(
        eval(&mut TestVM::new(),
             &TCon(box TBool(true),
                   box TString(String::from("Hello")),
                   box TString(String::from("World")))),
        TString(String::from("Hello")))
}
