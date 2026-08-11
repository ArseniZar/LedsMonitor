export type ApiSuccess<T>  = {
    ok: boolean;
    result: T;
};

export type ApiError  = {
    ok: false;
    error_code: number;
    message: string;
};

export type ResultData<T> = {
  status: boolean;
  value?: T;
  alertMessage?: string;
}