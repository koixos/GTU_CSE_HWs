;; loads lines of the file into memory as a list
(defun load-file (file)
    (with-open-file (stream file)
        (loop for line = (read-line stream nil)
                while line
                collect line)))
        
;; reads the next line from the loaded file recursively
;; returns a list with the line & next index
;; returns nil when reaching the EOF
(defun read-file (file-content curr-ind)
    (if (< curr-ind (length file-content))
        (list (nth curr-ind file-content) (1+ curr-ind))
        (list nil curr-ind)))

;; writes the whole converted code to the output file            
(defun write-file (line file)
    (with-open-file (output-stream file
                    :direction :output
                    :element-type 'character
                    :if-exists :append
                    :if-does-not-exist :create)
        (write-string line output-stream)
        (write-char #\Newline output-stream)))

(defun line-type (line)
    (let ((first-word (subseq line 0 (or (position #\Space line) (length line)))))
    (cond
        ((or (search "for" line)
            (search "while" line)) "loop")

        ((search "if" line) "if-statement")

        ((search "return" line) "return")

        ((and (search "=" line) 
            (search "(" line)
            (search ")" line)
            (search ";" line)
            (not (search "{" line))) "function-return")

        ((and (member first-word '("int" "double" "char") :test #'string=)
            (search "(" line)
            (search ")" line)
            (search ";" line)
            (not (search "=" line))) "function-prototype")

        ((and (search "(" line)
            (search ")" line)
            (search ";" line)
            (not (member first-word '("int" "double" "char") :test #'string=))) "function-call")

        ((and (search "(" line)
            (search ")" line)
            (search "{" line)) "function")

        ((or (search "+" line)
            (search "-" line)
            (search "*" line)
            (search "/" line)
            (search "&&" line)
            (search "||" line)
            (search "==" line)
            (search "!=" line)) "operation")

        ((search "=" line) "assignment")

        (t "parantheses"))))

;; decides the which conversion function will
;;  be used according to line type 
(defun conversion-foo (line_type)
    (cond 
        ((string= line_type "if-statement") 'convert-if)
        ((string= line_type "operation") 'convert-operations)
        ((string= line_type "loop") 'convert-loops)
        ((string= line_type "assignment") 'convert-assignments)
        ((string= line_type "function") 'convert-functions)
        ((string= line_type "function-call") 'convert-calls)
        ((string= line_type "function-return") 'convert-function-returns)
        ((string= line_type "function-prototype") 'convert-prototypes)
        ((string= line_type "return") 'convert-returns)
        ((string= line_type "parantheses") 'convert-parantheses)
        (t 'unknown)))

(defun convert (line func)
    (let ((converted-line (funcall func line)))
        (write-file converted-line "Output.lsp")))

;; functions for line types
(defun convert-if (line)
    (let* ((start (search "(" line))
            (end (search ")" line :start2 start))
            (condition (when (and start end)
                        (subseq line (1+ start) end)))
            (lisp-condition (when condition
                                (convert-operations (replace-operators condition)))))
        (if lisp-condition
            (format nil "(if (~a)" lisp-condition)
            "Error: Invalid if statement")))

(defun convert-operations (line)
    (let* ((line (string-trim " " line))
            (operands (split-sequence #\Space line))
            (operator (second operands)))
        (format nil "~a ~a ~a" operator (first operands) (third operands))))

(defun convert-loops (line)
    (cond
        ;; for FOR LOOPs
        ((and (search "for" line)
            (search "(" line)
            (search ";" line))
        (let* ((start (search "(" line))
                (end (search ")" line :start2 start))
                (loop-params (subseq line (1+ start) end))
                (parts (split-sequence #\; loop-params)))
            (when (= (length parts) 3)
                (let* ((init (first parts))
                        (cond (second parts))
                        (step (third parts))
                        (var (second (split-sequence #\Space init)))
                        (start-val (if (and (third (split-sequence #\Space init))
                                            (parse-integer (third (split-sequence #\Space init)) :junk-allowed t))
                                        (parse-integer (third (split-sequence #\Space init)))
                                        0))
                        ;;(cond-val (second (split-sequence #\Space cond)))
                        (limit (fourth (split-sequence #\Space cond)))
                        (direction (if (search "++" step) "from" "downto")))
                    
                    (cond
                            ((search ">" line)
                                (format nil "(loop for ~a from ~a to ~a do " var start-val limit))
                            ((search "<" line)
                                (format nil "(loop for ~a from ~a below ~a do " var start-val limit))
                            (t (format nil "~a - ~a - ~a " var start-val limit)))))))
        ;; for WHILE LOOPs
        ((search "while" line)
            (let* ((start (search "(" line))
                    (end (search ")" line :start2 start))
                    (condition (when (and start end)
                                (subseq line (1+ start) end)))
                    (lisp-condition (replace-operators condition)))
                (when lisp-condition
                    (format nil "(loop while ~a do " lisp-condition))))

        (t "Error: Invalid loop statement")))

(defun convert-assignments (line)
    (when (search "=" line)
        (let* ((parts (split-sequence #\= line))
                (var-part (first parts))
                (val-part (second parts)))
            
            (let* ((var (second (split-sequence #\Space var-part)))
                    (val (string-trim " ;" val-part)))
                (format nil "(let ((~a ~a))" var val)))))

(defun convert-functions (line)
    (when (search "(" line)
        (let* ((name-end (search "(" line))
                (name-part (split-sequence #\Space (string-trim " " (subseq line 0 name-end))))
                (name (second name-part))
                (params-start (search "(" line))
                (params-end (search ")" line))
                (params (when (and params-start params-end)
                            (subseq line (1+ params-start) params-end)))
                (param-list (when params
                                (remove-if (lambda (word)
                                                (member word '("int" "double" "char") :test 'string=))
                                            (split-sequence #\Space (substitute #\Space #\, params))))))
            (format nil "(defun ~a ~a" name param-list))))

(defun convert-function-returns (line)
    (let* ((equal-pos (search "=" line))
            (func-call (subseq line (1+ equal-pos)))
            (name-part (split-sequence #\Space (string-trim '(#\Space) (subseq line 0 equal-pos))))
            (name (second name-part))
            (func-name-end (search "(" func-call))
            (func-name (subseq func-call 0 func-name-end))
            (args-start (1+ func-name-end))
            (args-end (search ")" func-call :start2 args-start))
            (args (when (and args-start args-end)
                    (split-sequence #\, (subseq func-call args-start args-end)))))
        (format nil "(let ((~a (~a ~{~a~^ ~})))" name func-name
            (mapcar (lambda (arg) (string-trim '(#\Space) arg)) args))))

(defun convert-prototypes (line)
    (let* ((name-end (search "(" line))
            (name-part (split-sequence #\Space (when name-end
                    (string-trim " " (subseq line 0 name-end)))))
            (name (second name-part))
            (params-start (1+ name-end))
            (params-end (search ")" line :start2 params-start))
            (params (when (and params-start params-end)
                        (split-sequence #\, (subseq line params-start params-end))))
            (param-types (mapcar (lambda (param)
                                    (cond ((search "int" param) 'integer)
                                            ((search "double" param) 'double-float)
                                            ((search "char" param) 'character)
                                            (t 'unknown)))
                                    params))
            (return-type (first name-part))
            (converted-return-type (cond ((string= "int" return-type) 'integer)
                                            ((string= "double" return-type) 'double-float)
                                            ((search "char" return-type) 'character)
                                            (t 'unknown))))
        (format nil "(declaim (ftype (function (~{~a~^ ~}) ~a) ~a))" param-types converted-return-type name)))

(defun convert-calls (line)
    (let* ((printf-call (search "printf" line))
            (start-paren (search "(" line))
            (end-paren (search ")" line :start2 start-paren))
            (args (when (and start-paren end-paren)
                    (subseq line (1+ start-paren) end-paren))))
        
        (cond
            (printf-call
                (let* ((format-str (string-trim '(#\" #\Space) (first (split-sequence #\, args))))
                        (replaced-format-str (replace-substring format-str "%d" "~d"))
                        (replaced-format-str (replace-substring replaced-format-str "%s" "~a"))
                        (replaced-format-str (replace-substring replaced-format-str "%f" "~f"))
                        (replaced-format-str (replace-substring replaced-format-str "%c" "~c"))
                        (replaced-format-str (replace-substring replaced-format-str "\\n" "~%"))
                        (vars (rest (split-sequence #\, args))))
                    (format nil "(format t \"~a\" ~{~a~^ ~})" replaced-format-str vars)))
            
            (t
                (let* ((name (string-trim "()" (subseq line 0 start-paren)))
                        (args-list (split-sequence #\, args)))
                    (format nil "(~a ~{~a~^ ~})" name args-list))))))

(defun convert-returns (line)
    (let* ((return-pos (search "return" line))
            (semicolon-pos (search ";" line :start2 return-pos))
            (val (when (and return-pos semicolon-pos)
                    (string-trim '(#\Space) (subseq line (+ return-pos 6) semicolon-pos)))))
        
        (if val
            val
            "Error: No return value found")))

;; function for invalid operations/lines
(defun unknown (line) "unknown")

(defun convert-parantheses (line) ")")

;; Extra functions
(defun replace-operators (condition)
    (let ((temp condition))
        (setf temp (replace-substring temp "&&" "and"))
        (setf temp (replace-substring temp "||" "or"))
        (setf temp (replace-substring temp "==" "="))
        (setf temp (replace-substring temp "!=" "/="))
        temp))

(defun replace-substring (str old new)
    (if (and str old new)
        (with-output-to-string (out)
            (let ((start 0))
                (loop for pos = (search old str :start2 start)
                    do (progn
                        (write-string str out :start start :end (or pos (length str)))
                        (when pos (write-string new out))
                        (setf start (if pos (+ pos (length old)) (length str))))
                    while pos)
                (when (< start (length str))
                    (write-string str out :start start))))
        str))

;; Function to split a sequence by a given delimeter & return a list of substrings
(defun split-sequence (delimeter sequence)
    (let ((result '())
            (start 0)
            (end (length sequence)))
        (loop for pos = (position delimeter sequence :start start)
            while pos
            do (progn
                (push (subseq sequence start pos) result)
                (setf start (1+ pos)))
            finally (push (subseq sequence start end) result))
        (nreverse result)))
    
;; Function to remove leading tabs & spaces in the input file
;; If there is only a space or a tab, returns empty string
(defun remove-tabs-spaces (line)
    (let ((start (position-if-not (lambda (ch) (member ch '(#\Space #\Tab))) line)))
        (if start
            (subseq line start)
            "")))

;; Function to join parameters together
(defun string-join (list separator)
    (reduce (lambda (x y) (format nil "~a~a~a" x separator y)) list))

(defun main ()
    (let ((content (load-file "Input.c")))
        (labels ((process-lines (curr-ind) 
            (let* ((lines (read-file content curr-ind))
                    (line (first lines))
                    (next-ind (second lines)))
                (when line
                    (let* ((type (line-type line))
                            (function-used (conversion-foo type))
                            (clean-line (remove-tabs-spaces line)))
                        (convert clean-line function-used))
                    (process-lines next-ind)))))
            (process-lines 0))))
    
(main)