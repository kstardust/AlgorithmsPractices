(defun is-gcd(x y)
    (if (equalp y 0)
	x
	(is-gcd y (mod x y))))

(defun ext-euclid(x y)
  (if (equalp y 0)
      `(:a 1 :b 0 :g ,x)
      (let ((res (ext-euclid y (mod x y))))
	`(:a ,(getf res :b)
	      :b ,(- (getf res :a) (* (getf res :b) (floor x y)))
	      :g ,(getf res :g)))))

(format t "GCD ~a~%" (ext-euclid 796 90))
