import { createEvent, createStore } from "effector";

// point table
const addPoint = createEvent();
const removePointById = createEvent();
const clearPointStore = createEvent();
const loadStore = createEvent();

const $pointStore = createStore([])
    // add front
    .on(addPoint, (state, point) => { return [point, ...state] })
    // remove 
    .on(removePointById, ((state, id) => {
        return state.filter(p => p.pointId !== id);
    }))
    // load reversed
    .on(loadStore, (state, points) => { return points.reverse(); })
    // clear
    .on(clearPointStore, (state) => [])


export { addPoint, removePointById, clearPointStore, loadStore, $pointStore };

// radius calculation
const setRadius = createEvent();
const $radiusStore = createStore(1)
    .on(setRadius, (s, r) => r );

export { setRadius, $radiusStore };


