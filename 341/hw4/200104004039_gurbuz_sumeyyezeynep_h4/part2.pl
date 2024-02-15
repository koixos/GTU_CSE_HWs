% classify predicate with decision rules
classify(SepalLength, SepalWidth, PetalLength, PetalWidth, Class) :-
    decide_petal_length(PetalLength, PetalWidth, Class).

% Decision rules based on PetalLength
decide_petal_length(PetalLength, PetalWidth, Class) :-
    (   PetalLength =< 2.45
    ->  Class = 'Iris-setosa'
    ;   decide_petal_width(PetalLength, PetalWidth, Class)
    ).

% Decision rules based on PetalWidth for non-setosa flowers
decide_petal_width(PetalLength, PetalWidth, Class) :-
    (   PetalWidth =< 1.75
    ->  decide_petal_length_and_width(PetalLength, PetalWidth, Class)
    ;   Class = 'Iris-virginica'
    ).

% Further decisions based on a combination of PetalLength and PetalWidth
decide_petal_length_and_width(PetalLength, PetalWidth, Class) :-
    (   PetalLength =< 4.95
    ->  (   PetalWidth =< 1.65
            ->  Class = 'Iris-versicolor'
            ;   Class = 'Iris-virginica'
            )
    ;   (   PetalWidth =< 1.55
            ->  Class = 'Iris-virginica'
            ;   Class = 'Iris-versicolor'
            )
    ).

% Example query
% classify(4.9, 2.4, 3.3, 1.0, Class).