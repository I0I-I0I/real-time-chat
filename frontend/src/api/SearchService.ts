import { DB_URL } from "@/consts"
import { IFetchData, IUser, IChat } from "@/types"

const URL = DB_URL + "/search"

type types = IUser | IChat

export default class SearchService {
    static async search<T extends types>(
        by: string,
        table: string,
        value: string
    ): Promise<T[] | null> {
        const resp = await fetch(URL + "?by=" + by + "&table=" + table + "&value=" + value, {
            method: "GET"
        })
        const data = await resp.json() as IFetchData<T>
        if (data.status !== 200) {
            return null
        }
        return data.data as T[]
    }
}
