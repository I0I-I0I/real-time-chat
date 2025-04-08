import { DB_URL } from "@/consts"
import { IFetchData, IPostUser, IUser, ICheckUser } from "@/types"

const URL = DB_URL + "/users"

export default class UserService {
    static async getById(id: number): Promise<IUser | null> {
        const resp = await fetch(URL + "?id=" + id, {
            method: "GET"
        })
        const data = await resp.json() as IFetchData<IUser>
        if (data.status !== "OK") {
            return null
        }
        return data.data[0]
    }

    static async getByLogin(login: string): Promise<IUser | null> {
        const resp = await fetch(URL + "?login=" + login, {
            method: "GET",
        })
        const data = await resp.json() as IFetchData<IUser>
        if (data.status !== "OK") {
            return null
        }
        return data.data[0]
    }

    static async getAll(): Promise<IUser[] | null> {
        const resp = await fetch(URL)
        const data = await resp.json() as IFetchData<IUser>
        if (data.status !== "OK") {
            return null
        }
        return data.data
    }

    static async createOne(post_data: IPostUser): Promise<IFetchData<IUser> | null>  {
        const resp = await fetch(URL, {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify([{
                login: post_data.login,
                username: post_data.username,
                password: post_data.password
            }])
        })
        const data = await resp.json() as IFetchData<IUser>
        if (data.status !== "OK") {
            if (data.message === "SQL: UNIQUE constraint failed: users.login")
                return data
            return null
        }
        return data
    }

    static async checkOne(check_data: ICheckUser): Promise<IFetchData<IUser> | null> {
        const resp = await fetch(URL + "?type=check", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify([{
                login: check_data.login,
                password: check_data.password
            }])
        })
        const data = await resp.json() as IFetchData<IUser>
        if (data.status !== "OK") {
            if (data.message === "Wrong password")
                return data
            return null
        }
        return data
    }

    static async removeOne(id: number): Promise<string | null>  {
        const resp = await fetch(URL + "?id=" + id, {
            method: "DELETE",
        })
        const data = await resp.json() as IFetchData<IUser>
        if (data.status === "OK") {
            return data.status
        }
        return null
    }
}
