import styles from "./Dropdown.module.css"
import cls from "@/utils/cls"
import React from "react"

interface DropdownProps {
    dropdownState: boolean
    children: React.ReactNode
    className?: string
}

interface DropdownItemProps {
    children: React.ReactNode,
    className?: string
}

export const Dropdown = ({
    dropdownState: dropdownHidden,
    className = "",
    children
}: DropdownProps): JSX.Element => (
    <ul className={cls(className, styles.dropdown, dropdownHidden ? "" : "hidden" )}>
        {children}
    </ul>
)

export const DropdownItem = ({
    children,
    className = ""
}: DropdownItemProps): JSX.Element => (
    <li className={cls(styles.dropdown_item, className)}>
        {children}
    </li>
)
