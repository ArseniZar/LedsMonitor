class NetworkError extends Error {
    constructor(message: string = "Connection error. Please check your internet and try again.") {
        super(message);
        this.name = 'NetworkError';
    }
}

class InvalidJsonError extends Error {
    constructor() {
        super('Invalid JSON response from server');
        this.name = 'InvalidJsonError';
    }
}


class HttpError extends Error {
    status: number;

    constructor(status: number, mess: string) {
        super(`HTTP Error ${mess}`);
        this.name = 'HttpError';
        this.status = status;
        this.message = mess;
    }
}


export { NetworkError, HttpError, InvalidJsonError }