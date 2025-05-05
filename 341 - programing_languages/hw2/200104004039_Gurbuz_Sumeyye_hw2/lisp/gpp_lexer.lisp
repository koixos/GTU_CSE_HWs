(defvar keyword-list '("and" "or" "not" "equal" "less" "nill" "list" "append" "concat" "set" "def" "for" "if" "load" "display" "true" "false" "exit"))
(defvar kw-token-list '("KW_AND" "KW_OR" "KW_NOT" "KW_EQUAL" "KW_LESS" "KW_NILL" "KW_LIST" "KW_APPEND" "KW_CONCAT" "KW_SET" "KW_DEF" "KW_FOR" "KW_IF""KW_LOAD" "KW_DISPLAY" "KW_TURE" "KW_FALSE" "KW_EXIT"))

(defvar operator-list '("+" "-" "/" "*" "(" ")" ","))
(defvar op-token-list '("OP_PLUS" "OP_MINUS" "OP_DIV" "OP_MULT" "OP_OP" "OP_CP" "OP_COMMA"))

(defvar comment ";;")
(defvar comm-token "COMMENT")

(defvar in-file "input.txt")
(defvar line-list nil) ;to hold the read lines, initially empty
(defvar word-list nil) ;to hold splitted words, initially empty

;for this function, I got some help from internet
;a recursive function
;whenever it sees the given separator, it returns a substring as a word
(defun split-into-words (line &optional (r nil))
    (let ((n (position " " line
                :from-end t
                :test #'(lambda (x y)
                    (find y x :test #'string=)))))
        (if n
            (split-into-words (subseq line 0 n) (cons (subseq line (1+ n)) r))
            (cons line r))))

;function to ignore tabs in the splitted words
;if a word has 0 length, this means it is either a tab or anything nil
;nil words are removed from word list
(defun ignore-tabs ()
    (loop for word in word-list
        do
        (if (= 0 (length word))
            (setf word-list (remove word word-list)))))

;function to tokenize each word
;first it checks if a given word is comment
;secondly it checks one-char words & tokenize them
;lastly it checks more-than-one-char words & tokenize them
(defun tokenize-line (line)
    (setq word-list (split-into-words line))
    (ignore-tabs)
    (loop for word in word-list
        do
        (if (string= word comment)
            (format t "~a~%" comm-token))
        do
        (progn
        (setq counter 0)
        (setq flag 0)
        (if (= (length word) 1)
            (progn
            (loop for op in operator-list
                do 
                (if (string= word op)
                (
                    format t "~a~%" (nth counter op-token-list)
                    (setq flag 1)))
                (incf counter))

            (if (= flag 0)
                (if (and (string>= word #\0) (string<= word #\9))
                    (format t "VALUEF~%")
                    (format t "IDENTIFIER~%"))))

            (progn
            (dotimes (i (length word))
                (let ((char (elt word i)))
                    (let ((index (position (string char) operator-list :test #'equal)))
                        (if index
                            (format t "~a~%" (nth index op-token-list))))
                    (if (and (char>= char #\0) (char<= char #\9))
                        (format t "VALUEF~%")))))))))

(defun read-from-terminal ())

(defun read-from-file (file-in)
    (let ((in (open file-in)))
        (when in
            (loop for line = (read-line in nil)
                while line do (push line line-list))
            (close in))))

;main function
;if a file name given, read from the file
;otherwise read from the terminal
(defun gppinterpreter (&optional file-in)
    (if (equal file-in nil)
        (let ((input (read-from-terminal))))
        (let ((input (read-from-file file-in)))))

    (loop for item in (reverse line-list)
        do (tokenize-line item)))

(gppinterpreter in-file)