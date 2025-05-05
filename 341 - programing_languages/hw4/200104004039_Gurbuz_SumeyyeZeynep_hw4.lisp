(defun variable-p (x)
    (and (stringp x)
       (char= (char x 0) #\X)))

(defun did-occured (var term substitutions)
  (cond ((equal var term) t)
        ((and (stringp term) 
              (variable-p term)
              (assoc term substitutions))
         (did-occured var (second (assoc term substitutions)) substitutions))
        ((listp term) 
         (some #'(lambda (subterm) 
                  (did-occured var subterm substitutions))
               term))
        (t nil)))

(defun check-circular-substitutions (new-subs all-subs)
  (some #'(lambda (sub)
           (let ((var (first sub))
                 (term (second sub)))
             (or (did-occured var term all-subs)
                 (and (stringp term)
                      (variable-p term)
                      (did-occured term var all-subs)))))
        new-subs))

(defun sort-predicate (predicate)
    (if (listp predicate)
        (cons (first predicate)
            (sort (copy-list (rest predicate))
                    #'(lambda (a b)
                        (cond ((and (variable-p a) (variable-p b)) (string< a b))
                            ((variable-p a) nil)
                            ((variable-p b) t)
                            (t (string< a b))))))
        predicate))

(defun unify (a b)
    (cond
        ((equal a b) nil)
        ((variable-p a)
            (if (and (listp b) (member a b))
                nil
                (list (list a b))))
        ((variable-p b)
            (if (and (listp a) (member b a)) 
                nil
                (list (list b a))))
        
        ((and (listp a) (listp b)
            (equal (length a) (length b)))
        (let ((results nil))
            (loop for x in a
                    for y in b
                    do (let ((sub (unify x y)))
                            (when sub
                                (setf results (append results sub)))))
            results))
        (t nil)))

(defun apply-substitutions (query substitutions)
    (mapcar (lambda (predicate)
                (mapcar (lambda (term)
                            (if (and (stringp term)
                                    (assoc term substitutions :test #'equal))
                                (second (assoc term substitutions :test #'equal))
                                term))
                        predicate))
            query))

(defun create-goal-flag (goal substitutions)
    (let ((applied-goal (apply-substitutions (list goal) substitutions)))
        (list (sort-predicate (first applied-goal)) substitutions)))

(defun resolve (axioms goal)
  (loop for axiom in axioms
        for rule = (first axiom)
        for unified = (unify goal rule)
        when unified
            collect
                (if (= (length axiom) 1)
                    (list unified nil)
                    (list unified (apply-substitutions (cddr axiom) unified)))))

(defun dfs (axioms goals substitutions visited &optional (depth 0))
    (if (> depth 10)
        nil
        (if (null goals)
            (list substitutions)
            (let* ((curr-goal (first goals))
                    (remaining-goals (rest goals))
                    (applied-goal (if substitutions
                                    (first (apply-substitutions (list curr-goal) substitutions))
                                    curr-goal))
                    (goal-flag (list applied-goal substitutions)))
                (if (member goal-flag visited :test #'equal)
                    nil
                    (let ((resolve-results (resolve axioms applied-goal)))
                        (loop for result in resolve-results
                            append (let* ((new-substitutions (first result))
                                        (new-goals (second result))
                                        (updated-substitutions (append new-substitutions substitutions)))
                                    (unless (check-circular-substitutions new-substitutions updated-substitutions)
                                        (dfs axioms (append new-goals remaining-goals)
                                                    updated-substitutions
                                                    (cons goal-flag visited)
                                                    (1+ depth)))))))))))

(defun prolog-prove (axioms query)
  (let ((result (dfs axioms query nil nil 0)))
    (when result
        (let ((vars (remove-duplicates
                    (remove-if-not #'variable-p (reduce #'append query)))))
            (remove-duplicates
                (mapcar #'(lambda (solution)
                            (mapcar #'(lambda (var)
                                        (list var
                                            (second (assoc var solution))))
                                    vars))
                        result)
                :test #'equal)))))

(let ((axioms '((("father" "jim" "jill"))
                (("mother" "mary" "jill"))
                (("father" "samm" "jim"))
                (("ancestor" "X" "Y") "<" ("parent" "X" "Y"))
                (("ancestor" "X" "Y") "<" ("ancestor" "X" "Z") ("ancestor" "Z" "Y"))
                (("parent" "X" "Y") "<" ("mother" "X" "Y"))
                (("parent" "X" "Y") "<" ("father" "X" "Y"))))
      (query1 '(("ancestor" "X" "jill")))
      (query2 '(("ancestor" "X" "jill") ("mother" "X" "bob"))))
  (format t "Query1 result: ~A~%" (prolog-prove axioms query1))
  (format t "Query2 result: ~A~%" (prolog-prove axioms query2)))